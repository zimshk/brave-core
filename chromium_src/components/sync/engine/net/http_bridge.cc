#include "../../../../../../components/sync/engine/net/http_bridge.cc"

namespace syncer {

void HttpBridge::SetMethod(const char* method) {
  request_method_.assign(method);
}

}  // namespace syncer
