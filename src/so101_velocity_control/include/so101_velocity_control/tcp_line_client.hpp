#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <mutex>
#include <optional>
#include <string>
#include <sys/time.h>

class TcpLineClient
{
public:
  ~TcpLineClient() { close(); }

  bool connect(const std::string & host, int port, std::string & error)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    closeLocked();
    fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ < 0) {
      error = std::string("socket failed: ") + std::strerror(errno);
      return false;
    }
    int enabled = 1;
    (void)::setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &enabled, sizeof(enabled));
    timeval timeout{0, 100000};
    (void)::setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(static_cast<uint16_t>(port));
    if (::inet_pton(AF_INET, host.c_str(), &address.sin_addr) != 1) {
      error = "invalid IPv4 address: " + host;
      closeLocked();
      return false;
    }
    if (::connect(fd_, reinterpret_cast<sockaddr *>(&address), sizeof(address)) != 0) {
      error = std::string("connect failed: ") + std::strerror(errno);
      closeLocked();
      return false;
    }
    connected_ = true;
    ++generation_;
    return true;
  }

  bool isConnected()
  {
    std::lock_guard<std::mutex> lock(mutex_);
    return connected_;
  }

  bool send(const std::string & data)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!connected_) {
      return false;
    }
    size_t offset = 0;
    while (offset < data.size()) {
      const auto count = ::send(fd_, data.data() + offset, data.size() - offset, MSG_NOSIGNAL);
      if (count <= 0) {
        connected_ = false;
        return false;
      }
      offset += static_cast<size_t>(count);
    }
    return true;
  }

  std::optional<std::string> readLine()
  {
    while (true) {
      int fd;
      size_t generation;
      {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!connected_) {
          return std::nullopt;
        }
        fd = fd_;
        generation = generation_;
      }
      if (generation != read_generation_) {
        read_buffer_.clear();
        read_generation_ = generation;
      }
      const size_t newline = read_buffer_.find('\n');
      if (newline != std::string::npos) {
        auto line = read_buffer_.substr(0, newline);
        read_buffer_.erase(0, newline + 1);
        return line;
      }
      char buffer[4096];
      const auto count = ::recv(fd, buffer, sizeof(buffer), 0);
      if (count < 0 && (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)) {
        return std::nullopt;
      }
      if (count <= 0) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (generation == generation_) {
          connected_ = false;
        }
        return std::nullopt;
      }
      read_buffer_.append(buffer, static_cast<size_t>(count));
      if (read_buffer_.size() > 65536) {
        read_buffer_.clear();
      }
    }
  }

  void close()
  {
    std::lock_guard<std::mutex> lock(mutex_);
    closeLocked();
  }

private:
  void closeLocked()
  {
    if (fd_ >= 0) {
      (void)::shutdown(fd_, SHUT_RDWR);
      (void)::close(fd_);
      fd_ = -1;
    }
    connected_ = false;
    ++generation_;
  }

  std::mutex mutex_;
  int fd_{-1};
  bool connected_{false};
  size_t generation_{0};
  size_t read_generation_{static_cast<size_t>(-1)};
  std::string read_buffer_;
};
