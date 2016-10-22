//
//  RomoteViewController.h
//  WS BLE
//
//  Created by 杨伟标 on 16/9/16.
//  Copyright © 2016年 waveshare. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SerialGATT.h"

@interface RomoteViewController : UIViewController<BTSmartSensorDelegate,UIAlertViewDelegate>

@property (strong, nonatomic) SerialGATT *sensor;
@property (weak, nonatomic) IBOutlet UIButton *Button_Low;
@property (weak, nonatomic) IBOutlet UIButton *Button_Medium;
@property (weak, nonatomic) IBOutlet UIButton *Button_High;
@property (weak, nonatomic) IBOutlet UIButton *Button_Forward;
@property (weak, nonatomic) IBOutlet UIButton *Button_Backward;
@property (weak, nonatomic) IBOutlet UIButton *Button_Left;
@property (weak, nonatomic) IBOutlet UIButton *Button_Right;
@property (weak, nonatomic) IBOutlet UIButton *Button_Reset;
@property (weak, nonatomic) IBOutlet UILabel *Label_State;

@property BOOL SettingState;
@property int Button_Index;

@property NSString *StringReceive;
@end
