#include "utils/test_main.hpp"
#include "utils/socket_listener.hpp"
#include "utils/Hub.hpp"
#include "utils/HubDeviceSelect.hpp"
#include "utils/HubDevice.hpp"

/// Test payload for the hub unittest.
struct TestData
{
    /// Where this messsage was coming from (identifier or hub source)
    int from;
    /// Something like a TTL, will be decremented by the default test-hub
    /// handlers.
    int payload;
};

typedef HubContainer<StructContainer<TestData>> TestHubData;
typedef FlowInterface<Buffer<TestHubData>> TestHubPortInterface;
typedef StateFlow<Buffer<TestHubData>, QList<1>> TestHubPort;
typedef GenericHubFlow<TestHubData> TestHubFlow;
typedef HubDeviceSelect<TestHubFlow> TestHubDeviceAsync;
