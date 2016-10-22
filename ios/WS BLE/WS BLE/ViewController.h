//
//  ViewController.h
//  WS BLE
//
//  Created by 杨伟标 on 16/9/5.
//  Copyright © 2016年 waveshare. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SerialGATT.h"

@interface ViewController : UIViewController<BTSmartSensorDelegate>

@property (strong, nonatomic) CBPeripheral *peripheral;
@property (strong, nonatomic) SerialGATT *sensor;
@property (weak, nonatomic) IBOutlet UILabel *Label1;
@property (weak, nonatomic) IBOutlet UILabel *Label2; 
@property (weak, nonatomic) IBOutlet UIButton *UartButton;
@property (weak, nonatomic) IBOutlet UIButton *RomoteControlButton;
@property (weak, nonatomic) IBOutlet UIButton *DeviceControlButton;

@end

