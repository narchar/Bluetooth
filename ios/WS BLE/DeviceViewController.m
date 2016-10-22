//
//  DeviceViewController.m
//  WS BLE
//
//  Created by 杨伟标 on 16/9/20.
//  Copyright © 2016年 waveshare. All rights reserved.
//

#import "DeviceViewController.h"
#import "UIImageColorSelect.h"

@interface DeviceViewController ()

@end

@implementation DeviceViewController

@synthesize sensor;
@synthesize RGBImage;
@synthesize tapGesture;
@synthesize resultImage;
@synthesize resultLabel;
@synthesize RGB_R;
@synthesize RGB_G;
@synthesize RGB_B;
@synthesize Button_Buzzer;
@synthesize Buzzer_State;
@synthesize OLEDTextField;
@synthesize StringReceive;
@synthesize Label_RTC;
@synthesize Label_ACC;
@synthesize Label_ADC;
@synthesize Label_JOY;
@synthesize Label_TEMP;
@synthesize Button_Clear;
@synthesize Button_Send;
@synthesize StringBuzzerON;
@synthesize StringBuzzerOFF;
@synthesize StringRGBLED;
@synthesize StringRTC;
@synthesize StringACC;
@synthesize StringADC;
@synthesize StringJOY;
@synthesize StringTEMP;

#define UIColorFromHex(s) [UIColor colorWithRed:(((s & 0xFF0000) >> 16))/255.0 green:(((s &0xFF00) >>8))/255.0 blue:((s &0xFF))/255.0 alpha:1.0]

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.sensor.delegate = self;
    
    UIBarButtonItem *leftBarButtonItem = [[UIBarButtonItem alloc]
                                          initWithTitle:NSLocalizedString(@"Button_Back", @"")
                                          style:UIBarButtonItemStylePlain
                                          target:self
                                          action:@selector(doClickBackAction)
                                          ];
    
    self.navigationItem.leftBarButtonItem = leftBarButtonItem;
    
    [NSTimer scheduledTimerWithTimeInterval:0.2 target:self selector:@selector(delayMethod) userInfo:nil repeats:NO];
    
    //RGB
    tapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(selectColorAction:)];
    [RGBImage addGestureRecognizer:tapGesture];
    [resultImage setBackgroundColor:UIColorFromHex(0x57C0FF)];
    
    //OLED
    OLEDTextField.delegate = self;
    
    //Init
    StringBuzzerON = NSLocalizedString(@"Button_BuzzerON", @"");
    StringBuzzerOFF = NSLocalizedString(@"Button_BuzzerOFF", @"");
    StringRGBLED = resultLabel.text = NSLocalizedString(@"Label_RGB_LED", @"");
    StringRTC    = Label_RTC.text = NSLocalizedString(@"Label_RTC", @"");
    StringACC    = Label_ACC.text = NSLocalizedString(@"Label_ACC", @"");
    StringADC    = Label_ADC.text = NSLocalizedString(@"Label_ADC", @"");
    StringJOY    = Label_JOY.text = NSLocalizedString(@"Label_JOY", @"");
    StringTEMP   = Label_TEMP.text = NSLocalizedString(@"Label_TEMP", @"");
    
    Buzzer_State = 0;
    [self SetBuzzerButtonTitle:Buzzer_State];
    
    [Button_Buzzer setTitle:NSLocalizedString(@"Button_BuzzerON", @"") forState:UIControlStateNormal];
    [Button_Clear setTitle:NSLocalizedString(@"Button_Clear", @"") forState:UIControlStateNormal];
    [Button_Send setTitle:NSLocalizedString(@"Button_Send", @"") forState:UIControlStateNormal];
    
    Button_Buzzer.enabled = false;
    Button_Clear.enabled = false;
    Button_Send.enabled = false;
    
    [Button_Buzzer  setTitleColor:[UIColor brownColor] forState:UIControlStateNormal];
    [Button_Clear   setTitleColor:[UIColor brownColor] forState:UIControlStateNormal];
    [Button_Send    setTitleColor:[UIColor brownColor] forState:UIControlStateNormal];
    
    //增加监听，当键盘出现或改变时收出消息
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(keyboardWillShow:)
                                                 name:UIKeyboardWillShowNotification
                                               object:nil];

}


//当键盘出现或改变时调用
- (void)keyboardWillShow:(NSNotification *)aNotification
{
    //获取键盘的高度
    NSDictionary *userInfo = [aNotification userInfo];
    NSValue *aValue = [userInfo objectForKey:UIKeyboardFrameEndUserInfoKey];
    CGRect keyboardRect = [aValue CGRectValue];
    int height = keyboardRect.size.height;
    
    CGRect frame = OLEDTextField.frame;
    
    int offset = frame.origin.y + OLEDTextField.frame.size.height - (self.view.frame.size.height - height);//键盘高度216
    
    NSTimeInterval animationDuration = 0.30f;
    [UIView beginAnimations:@"ResizeForKeyboard" context:nil];
    [UIView setAnimationDuration:animationDuration];
    
    //将视图的Y坐标向上移动offset个单位，以使下面腾出地方用于软键盘的显示
    if(offset > 0)
        self.view.frame = CGRectMake(0.0f, -offset, self.view.frame.size.width, self.view.frame.size.height);
    
    [UIView commitAnimations];
}


- (void)doClickBackAction
{
    if (sensor.activePeripheral.state == CBPeripheralStateConnected)
    {
        [sensor disconnect:sensor.activePeripheral];
    }
    self.sensor.delegate = nil;
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)delayMethod
{
    [sensor connect:sensor.activePeripheral];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)selectColorAction:(UITapGestureRecognizer *)sender {
}
/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/
#pragma mark - Buzzer

- (void)SetBuzzerButtonTitle:(int) State
{
    if (State == 0)
    {
        [Button_Buzzer setTitle:StringBuzzerON forState:UIControlStateNormal];
    }
    else if (State == 1)
    {
        [Button_Buzzer setTitle:StringBuzzerOFF forState:UIControlStateNormal];
    }
}

- (IBAction)Button_Buzzer_TouchDown:(UIButton *)sender {
 
    if (Buzzer_State == 0)
    {
        NSString *str = @"{\"BZ\":\"on\"}";
        NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
        [sensor write:sensor.activePeripheral data:data];
        [self SetBuzzerButtonTitle:Buzzer_State];
    }
    else if(Buzzer_State == 1)
    {
        NSString *str = @"{\"BZ\":\"off\"}";
        NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
        [sensor write:sensor.activePeripheral data:data];
        [self SetBuzzerButtonTitle:Buzzer_State];
    }
}

#pragma mark - OLED


- (IBAction)OLED_Clear_TouchDown:(UIButton *)sender
{
    OLEDTextField.text = @"";
}


- (IBAction)OLED_Send_TouchDown:(UIButton *)sender
{
    NSString *temp = OLEDTextField.text;
    if (temp.length)
    {
        NSString *str = [NSString stringWithFormat:@"{\"OLED\":\"%@\"}",temp];
    
        NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
        [sensor write:sensor.activePeripheral data:data];
    }
}

//当用户按下return键或者按回车键，keyboard消失
-(BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    return YES;
}

//输入框编辑完成以后，将视图恢复到原始状态
-(void)textFieldDidEndEditing:(UITextField *)textField
{
    self.view.frame =CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height);
}


#pragma mark - RGB

//完成选择
-(void)finishedAction:(CGPoint)point
{
    UIColor *tempColor = [RGBImage.image colorAtPixel:point];
    
    
    CGFloat R,G,B;
    CGColorRef color = [tempColor CGColor];
    size_t numComponents = CGColorGetNumberOfComponents(color);
    
    if (numComponents == 4)
    {
        const CGFloat *components = CGColorGetComponents(color);
        R = components[0];
        G = components[1];
        B = components[2];
    }

    RGB_R = R*255;
    RGB_G = G*255;
    RGB_B = B*255;
    
    NSString *str = [NSString stringWithFormat:@"%@:%d %d %d",StringRGBLED,RGB_R,RGB_G,RGB_B];
    
    if(!((RGB_R==0)&&(RGB_G==0)&&(RGB_B==0)))
    {
        if ((RGB_R==255)||(RGB_G==255)||(RGB_B==255))
        {
            if (!((RGB_R>255)||(RGB_G>255)||(RGB_B>255)))
            {
                resultLabel.text = str;
                [resultImage setBackgroundColor:tempColor];
            }
        }
    }
}

//Touch
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint tempPoint = [touch locationInView:RGBImage];
    [self finishedAction:tempPoint];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    if(!((RGB_R==0)&&(RGB_G==0)&&(RGB_B==0)))
    {
        if ((RGB_R==255)||(RGB_G==255)||(RGB_B==255))
        {
            if (!((RGB_R>255)||(RGB_G>255)||(RGB_B>255)))
            {
                NSString *str = [NSString stringWithFormat:@"{\"RGB\":\"%d,%d,%d\"}",RGB_R,RGB_G,RGB_B];
                NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
                [sensor write:sensor.activePeripheral data:data];
            }
        }
    }
}


#pragma mark - BleDelegate
-(void) peripheralFound:(CBPeripheral *)peripheral
{
}

//recv data
-(void) serialGATTCharValueUpdated:(NSString *)UUID value:(NSData *)data
{
    NSString *value = [[NSString alloc] initWithData:data encoding:NSASCIIStringEncoding];
    
    NSRange range;
    range = [value rangeOfString:@"{"];
    if (range.location != NSNotFound)
    {
        StringReceive = @"";
    }
    
    range = [value rangeOfString:@"}"];
    if (range.location != NSNotFound)
    {
        StringReceive = [StringReceive stringByAppendingString:value];
        
        range = [value rangeOfString:@"{"];
        if (range.location != NSNotFound)
        {
            NSError *error;
            
            //将数据放到NSData对象中
            NSData *response =[StringReceive dataUsingEncoding:NSUTF8StringEncoding];
            
            //IOS5自带解析类NSJSONSerialization从response中解析出数据放到字典中
            NSDictionary *weatherDic = [NSJSONSerialization JSONObjectWithData:response options:NSJSONReadingMutableLeaves error:&error];
                       
            NSString *RTC_Sting = [NSString stringWithFormat:@"%@",[weatherDic objectForKey:@"RTC"]];
            if(![RTC_Sting isEqualToString:@"(null)"])
            {
                Label_RTC.text = [NSString stringWithFormat:@"%@:%@",StringRTC,RTC_Sting];
            }
         
            NSString *ACC_Sting = [NSString stringWithFormat:@"%@",[weatherDic objectForKey:@"ACC"]];
            if(![ACC_Sting isEqualToString:@"(null)"])
            {
                Label_ACC.text = [NSString stringWithFormat:@"%@:%@",StringACC,ACC_Sting];
            }
            
            NSString *ADC_Sting = [NSString stringWithFormat:@"%@",[weatherDic objectForKey:@"ADC"]];
            if(![ADC_Sting isEqualToString:@"(null)"])
            {
                Label_ADC.text = [NSString stringWithFormat:@"%@:%@",StringADC,ADC_Sting];
                NSLog(@"%@:%@",StringADC,ADC_Sting);
            }
            
            NSString *JOY_Sting = [NSString stringWithFormat:@"%@",[weatherDic objectForKey:@"JOY"]];
            if(![JOY_Sting isEqualToString:@"(null)"])
            {
                Label_JOY.text = [NSString stringWithFormat:@"%@:%@",StringJOY,JOY_Sting];
            }
            
            NSString *TEMP_Sting = [NSString stringWithFormat:@"%@",[weatherDic objectForKey:@"TEMP"]];
            if(![TEMP_Sting isEqualToString:@"(null)"])
            {
                Label_TEMP.text = [NSString stringWithFormat:@"%@:%@",StringTEMP,TEMP_Sting];
            }
            
            NSString *BZ_Sting = [NSString stringWithFormat:@"%@",[weatherDic objectForKey:@"BZ"]];
            if(![BZ_Sting isEqualToString:@"(null)"])
            {
                if ([BZ_Sting isEqualToString:@"on"])
                {
                    Buzzer_State = 1;
                    [self SetBuzzerButtonTitle:Buzzer_State];
                }
                else if ([BZ_Sting isEqualToString:@"off"])
                {
                    Buzzer_State = 0;
                    [self SetBuzzerButtonTitle:Buzzer_State];
                }
            }
        }
    }
    else
    {
        StringReceive = [StringReceive stringByAppendingString:value];
    }
}

-(void)setConnect
{
    Button_Buzzer.enabled = true;
    Button_Clear.enabled = true;
    Button_Send.enabled = true;
    [Button_Buzzer  setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [Button_Clear   setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [Button_Send    setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
}

-(void)setDisconnect
{
    [sensor disconnect:sensor.activePeripheral];
    self.sensor.delegate = nil;
    [self.navigationController popViewControllerAnimated:YES];
    
    UIAlertView * alert = [[UIAlertView alloc]initWithTitle:
                           NSLocalizedString(@"Connection_Alert_Title", @"")
                                                    message:NSLocalizedString(@"Connection_Alert_Content", @"")
                                                   delegate:nil
                                          cancelButtonTitle:NSLocalizedString(@"Connection_Alert_Button", @"")
                                          otherButtonTitles:nil];
    
    [alert show];
}

@end
