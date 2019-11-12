#include "iOSSyncDevice.h"
#include <memory>
#include "components/brave_sync/sync_device.h"

@interface iOSSyncDevice()
@property (nonatomic, strong) std::unique_ptr<SyncDevice> device;
@end

@implementation iOSSyncDevice
- (id)init {
    if ((self = [super init])) {
        _device = std::make_unique<SyncDevice>();
    }
    return self;
}

- (id)init:(NSString *)name objectId:(NSString *)objectId deviceId:(NSString *)deviceId lastActiveTimestamp:(NSTimeInterval)lastActiveTimestamp {
    
    if ((self = [super init])) {
        _device = std::make_unique<SyncDevice>([name utf8String],
                                               [objectId utf8String],
                                               [deviceId utf8String],
                                               lastActiveTimestamp);
    }
    return self;
}

- (NSString *)name {
    return [[NSString alloc] initWithUTF8String:device->name_.c_str()];
}

- (NSString *)objectId {
    return [[NSString alloc] initWithUTF8String:device->object_id_.c_str()];
}

- (NSString *)deviceId {
    return [[NSString alloc] initWithUTF8String:device->device_id_.c_str()];
}

- (NSTimeInterval)lastActiveTimestamp {
    return device->last_active_ts_;
}
@end

meh!!
