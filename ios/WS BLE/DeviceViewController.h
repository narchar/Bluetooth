//
//  DeviceViewController.h
//  WS BLE
//
//  Created by 杨伟标 on 16/9/20.
//  Copyright © 2016年 waveshare. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SerialGATT.h"

@interface DeviceViewController : UIViewController<BTSmartSensorDelegate,UITextFieldDelegate>

@property (strong, nonatomic) SerialGATT *sensor;


@property (weak, nonatomic) IBOutlet UIImageView *RGBImage;

@property (weak, nonatomic) IBOutlet UIImageView *resultImage;
@property (weak, nonatomic) IBOutlet UILabel *resultLabel;

@property (strong, nonatomic) UITapGestureRecognizer *tapGesture;

@property (weak, nonatomic) IBOutlet UIButton *Button_Buzzer;
@property unsigned int Buzzer_State;

@property unsigned int RGB_R;
@property unsigned int RGB_G;
@property unsigned int RGB_B;

//输入
@property (weak, nonatomic) IBOutlet UITextField *OLEDTextField;

//接收
@property NSString *StringReceive;

@property (weak, nonatomic) IBOutlet UILabel *Label_RTC;
@property (weak, nonatomic) IBOutlet UILabel *Label_ACC;
@property (weak, nonatomic) IBOutlet UILabel *Label_ADC;
@property (weak, nonatomic) IBOutlet UILabel *Label_JOY;
@property (weak, nonatomic) IBOutlet UILabel *Label_TEMP;
@property (weak, nonatomic) IBOutlet UIButton *Button_Clear;
@property (weak, nonatomic) IBOutlet UIButton *Button_Send;

@property NSString *StringBuzzerON;
@property NSString *StringBuzzerOFF;
@property NSString *StringRGBLED;
@property NSString *StringRTC;
@property NSString *StringACC;
@property NSString *StringADC;
@property NSString *StringJOY;
@property NSString *StringTEMP;

@end



