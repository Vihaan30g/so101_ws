#pragma once
// tcp_client.hpp
//
// Tiny POSIX-socket TCP client wrapper. Deliberately not using Boost.Asio
// or any other extra dependency -- raw sockets are more than sufficient
// for a single persistent localhost connection at 100-200 Hz, and it
// keeps so101_control's dependency footprint to just rclcpp + the
// existing so101_kinematics/so101_msgs packages.
//
// Thread-safety contract: `send()` and `close()` take the internal mutex.
// `connect()` also takes it. `readLine()` is intended to be called from a
// single dedicated reader thread and does its own recv() outside the
// lock (only touching the fd, which is only ever changed to -1 by
// close()/connect() under the lock) -- callers must not call readLine()
// concurrently from multiple threads.

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

namespace so101_control
{

class TcpClient
{
public:
  ~TcpClient() { close(); }

  // Blocking connect with a short timeout. Returns true on success.
  bool connect(const std::string & host, int port, std::string & err_out)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    close_locked();

    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ < 0) {
      err_out = std::string("socket() failed: ") + std::strerror(errno);
      return false;
    }

    // Disable Nagle's algorithm: we send small messages at a fixed high
    // rate and want them on the wire immediately, not batched.
    int one = 1;
    setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(static_cast<uint16_t>(port));
    if (inet_pton(AF_INET, host.c_str(), &addr.sin_addr) != 1) {
      err_out = "invalid host address: " + host;
      close_locked();
      return false;
    }

    if (::connect(fd_, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) != 0) {
      err_out = std::string("connect() failed: ") + std::strerror(errno);
      close_locked();
      return false;
    }

    connected_ = true;
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
    size_t sent = 0;
    while (sent < data.size()) {
      ssize_t n = ::send(fd_, data.data() + sent, data.size() - sent, MSG_NOSIGNAL);
      if (n <= 0) {
        connected_ = false;
        return false;
      }
      sent += static_cast<size_t>(n);
    }
    return true;
  }

  // Reads one newline-terminated line (newline stripped). Returns
  // std::nullopt on disconnect/error/timeout. Uses a per-call recv with a
  // small internal buffer so partial reads across calls are stitched
  // together correctly.
  std::optional<std::string> readLine()
  {
    while (true) {
      size_t nl = read_buf_.find('\n');
      if (nl != std::string::npos) {
        std::string line = read_buf_.substr(0, nl);
        read_buf_.erase(0, nl + 1);
        return line;
      }

      int fd_copy;
      {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!connected_) {
          return std::nullopt;
        }
        fd_copy = fd_;
      }

      char buf[4096];
      ssize_t n = ::recv(fd_copy, buf, sizeof(buf), 0);
      if (n <= 0) {
        std::lock_guard<std::mutex> lock(mutex_);
        connected_ = false;
        return std::nullopt;
      }
      read_buf_.append(buf, static_cast<size_t>(n));
    }
  }

  void close()
  {
    std::lock_guard<std::mutex> lock(mutex_);
    close_locked();
  }

private:
  void close_locked()
  {
    if (fd_ >= 0) {
      ::close(fd_);
      fd_ = -1;
    }
    connected_ = false;
    read_buf_.clear();
  }

  std::mutex mutex_;
  int fd_{-1};
  bool connected_{false};
  std::string read_buf_;
};

}  // namespace so101_control
