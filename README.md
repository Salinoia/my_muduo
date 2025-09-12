# Muduo Plus

Muduo Plus is a modular C++ network framework built on top of a core event-driven library. The project demonstrates a layered design with protocol modules, infrastructure utilities and sample services.

## Directory Structure

```
.
├── CMakeLists.txt          # Top level build script
├── examples/               # Demo applications
├── src/
│   ├── config/             # YAML/JSON configuration files
│   ├── core/               # Event loop, TCP server and other low level primitives
│   ├── framework/          # IoC container, router, session and utilities
│   ├── modules/            # Protocol implementations (HTTP, KCP, QUIC, WebSocket)
│   ├── services/           # Sample business logic built on the framework
│   └── storage/            # Data access layer: cache, database, message queue
├── tests/                  # Unit tests for framework components
└── third_party/            # External headers (e.g. nlohmann/json)
```

## Build and Test

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

## Components

- **Core** – provides the reactor pattern based on epoll with abstractions such as `EventLoop`, `TcpServer` and `Buffer`.
- **Framework** – common facilities including an IoC container, HTTP router, session management and thread utilities.
- **Modules** – protocol specific libraries (HTTP server, KCP/QUIC transports, WebSocket support) built atop the core.
- **Storage** – wrappers for Redis cache, RabbitMQ, and a simple ORM with connection pooling.
- **Services** – example services and controllers using framework and storage layers.

## Examples

Example applications under `examples/` illustrate how to combine the pieces: an echo server, HTTP demos, router usage and more.

## Testing

The project includes unit tests located in `tests/`, covering utilities like the ring buffer, router and configuration manager.
