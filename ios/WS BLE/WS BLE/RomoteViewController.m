//
//  RomoteViewController.m
//  WS BLE
//
//  Created by 杨伟标 on 16/9/16.
//  Copyright © 2016年 waveshare. All rights reserved.
//

#import "RomoteViewController.h"

@interface RomoteViewController ()

@end

@implementation RomoteViewController

@synthesize sensor;
@synthesize Button_Low;
@synthesize Button_Medium;
@synthesize Button_High;
@synthesize Button_Forward;
@synthesize Button_Backward;
@synthesize Button_Left;
@synthesize Button_Right;
@synthesize Button_Reset;
@synthesize Button_Index;
@synthesize SettingState;
@synthesize StringReceive;
@synthesize Label_State;

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

    SettingState = false;
    
    Button_Forward.enabled = false;
    Button_Backward.enabled = false;
    Button_Left.enabled = false;
    Button_Right.enabled = false;
    Button_Low.enabled = false;
    Button_Medium.enabled = false;
    Button_High.enabled = false;
    
    Button_Reset.hidden = YES;
    
    Button_Index = 0;
    
    [Button_Low setTitle:NSLocalizedString(@"Button_Low", @"") forState:UIControlStateNormal];
    [Button_Medium setTitle:NSLocalizedString(@"Button_Medium", @"") forState:UIControlStateNormal];
    [Button_High setTitle:NSLocalizedString(@"Button_High", @"") forState:UIControlStateNormal];
    [Button_Reset setTitle:NSLocalizedString(@"Button_Reset", @"") forState:UIControlStateNormal];
    
    Label_State.text = NSLocalizedString(@"Label_State", @"");
    
    NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
    NSString *str;
    
    str= [user objectForKey:@"FORWARD_DOWN"];
    if (!str)
    {
        [user setObject:@"{\"Forward\":\"Down\"}" forKey:@"FORWARD_DOWN"];
    }
    
    str= [user objectForKey:@"FORWARD_UP"];
    if (!str)
    {
        [user setObject:@"{\"Forward\":\"Up\"}" forKey:@"FORWARD_UP"];
    }

    str= [user objectForKey:@"BACKWARD_DOWN"];
    if (!str)
    {
        [user setObject:@"{\"Backward\":\"Down\"}" forKey:@"BACKWARD_DOWN"];
    }
    
    str= [user objectForKey:@"BACKWARD_UP"];
    if (!str)
    {
        [user setObject:@"{\"Backward\":\"Up\"}" forKey:@"BACKWARD_UP"];
    }
    
    str= [user objectForKey:@"LEFT_DOWN"];
    if (!str)
    {
        [user setObject:@"{\"Left\":\"Down\"}" forKey:@"LEFT_DOWN"];
    }
    
    str= [user objectForKey:@"LEFT_UP"];
    if (!str)
    {
        [user setObject:@"{\"Left\":\"Up\"}" forKey:@"LEFT_UP"];
    }
    
    str= [user objectForKey:@"RIGHT_DOWN"];
    if (!str)
    {
        [user setObject:@"{\"Right\":\"Down\"}" forKey:@"RIGHT_DOWN"];
    }
    
    str= [user objectForKey:@"RIGHT_UP"];
    if (!str)
    {
        [user setObject:@"{\"Right\":\"Up\"}" forKey:@"RIGHT_UP"];
    }

    str= [user objectForKey:@"LOW_DOWN"];
    if (!str)
    {
        [user setObject:@"{\"Low\":\"Down\"}" forKey:@"LOW_DOWN"];
    }
    
    str= [user objectForKey:@"MEDIUM_DOWN"];
    if (!str)
    {
        [user setObject:@"{\"Medium\":\"Down\"}" forKey:@"MEDIUM_DOWN"];
    }
    
    str= [user objectForKey:@"HIGH_DOWN"];
    if (!str)
    {
        [user setObject:@"{\"High\":\"Down\"}" forKey:@"HIGH_DOWN"];
    }
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

#define UIColorFromHex(s) [UIColor colorWithRed:(((s & 0xFF0000) >> 16))/255.0 green:(((s &0xFF00) >>8))/255.0 blue:((s &0xFF))/255.0 alpha:1.0]

- (void)doClickSettingAction
{
    if (SettingState)
    {
        Button_Index = 0;
        SettingState = false;
        Button_Reset.hidden = YES;
        self.navigationItem.rightBarButtonItem.title = NSLocalizedString(@"Button_Setting", @"");
        [Button_Forward setBackgroundImage:[UIImage imageNamed:@"button_forward_blue"] forState:UIControlStateNormal];
        [Button_Backward setBackgroundImage:[UIImage imageNamed:@"button_backward_blue"] forState:UIControlStateNormal];
        [Button_Left setBackgroundImage:[UIImage imageNamed:@"button_left_blue"] forState:UIControlStateNormal];
        [Button_Right setBackgroundImage:[UIImage imageNamed:@"button_right_blue"] forState:UIControlStateNormal];
        
        [Button_Low setTitleColor:[UIColor whiteColor]forState:UIControlStateNormal];
        [Button_Medium setTitleColor:[UIColor whiteColor]forState:UIControlStateNormal];
        [Button_High setTitleColor:[UIColor whiteColor]forState:UIControlStateNormal];
    }
    else
    {
        SettingState = true;
        Button_Reset.hidden = NO;
        self.navigationItem.rightBarButtonItem.title = NSLocalizedString(@"Button_Done", @"");
        [Button_Forward setBackgroundImage:[UIImage imageNamed:@"button_forward_green"] forState:UIControlStateNormal];
        [Button_Backward setBackgroundImage:[UIImage imageNamed:@"button_backward_green"] forState:UIControlStateNormal];
        [Button_Left setBackgroundImage:[UIImage imageNamed:@"button_left_green"] forState:UIControlStateNormal];
        [Button_Right setBackgroundImage:[UIImage imageNamed:@"button_right_green"] forState:UIControlStateNormal];
      
        [Button_Low setTitleColor:UIColorFromHex(0x11AA11)forState:UIControlStateNormal];
        [Button_Medium setTitleColor:UIColorFromHex(0x11AA11)forState:UIControlStateNormal];
        [Button_High setTitleColor:UIColorFromHex(0x11AA11)forState:UIControlStateNormal];
    }
    
}

- (void)delayMethod
{
    [sensor connect:sensor.activePeripheral];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)ButtonReset_TouchUpInside:(UIButton *)sender {
    NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
    [user setObject:@"{\"Forward\":\"Down\"}" forKey:@"FORWARD_DOWN"];
    [user setObject:@"{\"Forward\":\"Up\"}" forKey:@"FORWARD_UP"];
    [user setObject:@"{\"Backward\":\"Down\"}" forKey:@"BACKWARD_DOWN"];
    [user setObject:@"{\"Backward\":\"Up\"}" forKey:@"BACKWARD_UP"];
    [user setObject:@"{\"Left\":\"Down\"}" forKey:@"LEFT_DOWN"];
    [user setObject:@"{\"Left\":\"Up\"}" forKey:@"LEFT_UP"];
    [user setObject:@"{\"Right\":\"Down\"}" forKey:@"RIGHT_DOWN"];
    [user setObject:@"{\"Right\":\"Up\"}" forKey:@"RIGHT_UP"];
    [user setObject:@"{\"Low\":\"Down\"}" forKey:@"LOW_DOWN"];
    [user setObject:@"{\"Medium\":\"Down\"}" forKey:@"MEDIUM_DOWN"];
    [user setObject:@"{\"High\":\"Down\"}" forKey:@"HIGH_DOWN"];
    
    UIAlertView * alert = [[UIAlertView alloc]initWithTitle:NSLocalizedString(@"Setting_Reset", @"")
        message:NSLocalizedString(@"Setting_Reset_Success", @"")
        delegate:nil
        cancelButtonTitle:@"OK"
        otherButtonTitles:nil];
    
    [alert show];
}

- (IBAction)ButtonForward_TouchDown:(UIButton *)sender {
    if (SettingState)
    {
        NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
        
        NSString *str1= [user objectForKey:@"FORWARD_DOWN"];
        NSString *str2= [user objectForKey:@"FORWARD_UP"];
        
        UIAlertView *alert = [[UIAlertView alloc]
            initWithTitle:NSLocalizedString(@"Setting_Title", @"")
            message:NSLocalizedString(@"Setting_Text", @"")
            delegate:self
            cancelButtonTitle:NSLocalizedString(@"Setting_Cancel", @"")
            otherButtonTitles:NSLocalizedString(@"Setting_OK", @"")
                              ,nil];
        
        alert.alertViewStyle = UIAlertViewStyleLoginAndPasswordInput;
        
        UITextField *textField1 = [alert textFieldAtIndex:0];
        textField1.keyboardType = UIKeyboardTypeASCIICapable;
        textField1.placeholder = NSLocalizedString(@"Setting_Keydown", @"");
        textField1.text = str1;
        
        UITextField *textField2 = [alert textFieldAtIndex:1];
        textField2.keyboardType = UIKeyboardTypeASCIICapable;
        textField2.placeholder = NSLocalizedString(@"Setting_Keyup", @"");
        textField2.secureTextEntry = NO;
        textField2.text = str2;
        [alert show];
        
        Button_Index = 1;
    }
    else
    {
        NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
        NSString *str = [user objectForKey:@"FORWARD_DOWN"];
        
        NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
        [sensor write:sensor.activePeripheral data:data];
    }
}
- (IBAction)ButtonForward_TouchUpInside:(UIButton *)sender {
    if (!SettingState)
    {
        NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
        NSString *str = [user objectForKey:@"FORWARD_UP"];

        NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
        [sensor write:sensor.activePeripheral data:data];
    }
}
- (IBAction)ButtonForward_TouchUpOutside:(UIButton *)sender {
    if (!SettingState)
    {
        NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
        NSString *str = [user objectForKey:@"FORWARD_UP"];
        NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
        [sensor write:sensor.activePeripheral data:data];
    }
}


- (IBAction)ButtonBackward_TouchDown:(UIButton *)sender {
    if (SettingState)
    {
        NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
        
        NSString *str1= [user objectForKey:@"BACKWARD_DOWN"];
        NSString *str2= [user objectForKey:@"BACKWARD_UP"];
        
        UIAlertView *alert = [[UIAlertView alloc]
                              initWithTitle:NSLocalizedString(@"Setting_Title", @"")
                              message:NSLocalizedString(@"Setting_Text", @"")
                              delegate:self
                              cancelButtonTitle:NSLocalizedString(@"Setting_Cancel", @"")
                              otherButtonTitles:NSLocalizedString(@"Setting_OK", @"")
                              ,nil];
        
        alert.alertViewStyle = UIAlertViewStyleLoginAndPasswordInput;
        
        UITextField *textField1 = [alert textFieldAtIndex:0];
        textField1.keyboardType = UIKeyboardTypeASCIICapable;
        textField1.placeholder = NSLocalizedString(@"Setting_Keydown", @"");
        textField1.text = str1;
        
        UITextField *textField2 = [alert textFieldAtIndex:1];
        textField2.keyboardType = UIKeyboardTypeASCIICapable;
        textField2.placeholder = NSLocalizedString(@"Setting_Keyup", @"");
        textField2.secureTextEntry = NO;
        textField2.text = str2;
        [alert show];
        
        Button_Index = 2;
    }
    else
    {
        NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
        NSString *str = [user objectForKey:@"BACKWARD_DOWN"];
        
        NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
        [sensor write:sensor.activePeripheral data:data];
    }
}

- (IBAction)ButtonBackward_TouchUpInside:(UIButton *)sender {
    if (!SettingState)
    {
        NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
        NSString *str = [user objectForKey:@"BACKWARD_UP"];
        
        NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
        [sensor write:sensor.activePeripheral data:data];
    }
}

- (IBAction)ButtonBackward_TouchUpOutside:(UIButton *)sender {
    if (!SettingState)
    {
        NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
        NSString *str = [user objectForKey:@"BACKWARD_UP"];
        
        NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
        [sensor write:sensor.activePeripheral data:data];
    }
}

- (IBAction)ButtonLeft_TouchDown:(UIButton *)sender {
    if (SettingState)
    {
        NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
        
        NSString *str1= [user objectForKey:@"LEFT_DOWN"];
        NSString *str2= [user objectForKey:@"LEFT_UP"];
        
        UIAlertView *alert = [[UIAlertView alloc]
                              initWithTitle:NSLocalizedString(@"Setting_Title", @"")
                              message:NSLocalizedString(@"Setting_Text", @"")
                              delegate:self
                              cancelButtonTitle:NSLocalizedString(@"Setting_Cancel", @"")
                              otherButtonTitles:NSLocalizedString(@"Setting_OK", @"")
                              ,nil];
        
        alert.alertViewStyle = UIAlertViewStyleLoginAndPasswordInput;
        
        UITextField *textField1 = [alert textFieldAtIndex:0];
        textField1.keyboardType = UIKeyboardTypeASCIICapable;
        textField1.placeholder = NSLocalizedString(@"Setting_Keydown", @"");
        textField1.text = str1;
        
        UITextField *textField2 = [alert textFieldAtIndex:1];
        textField2.keyboardType = UIKeyboardTypeASCIICapable;
        textField2.placeholder = NSLocalizedString(@"Setting_Keyup", @"");
        textField2.secureTextEntry = NO;
        textField2.text = str2;
        [alert show];
        
        Button_Index = 3;
    }
    else
    {
        NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
        NSString *str = [user objectForKey:@"LEFT_DOWN"];
        
        NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
        [sensor write:sensor.activePeripheral data:data];
    }
}

- (IBAction)ButtonLeft_TouchUpInside:(UIButton *)sender {
    if (!SettingState)
    {
        NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
        NSString *str = [user objectForKey:@"LEFT_UP"];
        
        NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
        [sensor write:sensor.activePeripheral data:data];
    }
}

- (IBAction)ButtonLeft_TouchOutside:(UIButton *)sender {
    if (!SettingState)
    {
        NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
        NSString *str = [user objectForKey:@"LEFT_UP"];
        
        NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
        [sensor write:sensor.activePeripheral data:data];
    }
}

- (IBAction)ButtonRight_TouchDown:(UIButton *)sender {
    if (SettingState)
    {
        NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
        
        NSString *str1= [user objectForKey:@"RIGHT_DOWN"];
        NSString *str2= [user objectForKey:@"RIGHT_UP"];
        
        UIAlertView *alert = [[UIAlertView alloc]
                              initWithTitle:NSLocalizedString(@"Setting_Title", @"")
                              message:NSLocalizedString(@"Setting_Text", @"")
                              delegate:self
                              cancelButtonTitle:NSLocalizedString(@"Setting_Cancel", @"")
                              otherButtonTitles:NSLocalizedString(@"Setting_OK", @"")
                              ,nil];
        
        alert.alertViewStyle = UIAlertViewStyleLoginAndPasswordInput;
        
        UITextField *textField1 = [alert textFieldAtIndex:0];
        textField1.keyboardType = UIKeyboardTypeASCIICapable;
        textField1.placeholder = NSLocalizedString(@"Setting_Keydown", @"");
        textField1.text = str1;
        
        UITextField *textField2 = [alert textFieldAtIndex:1];
        textField2.keyboardType = UIKeyboardTypeASCIICapable;
        textField2.placeholder = NSLocalizedString(@"Setting_Keyup", @"");
        textField2.secureTextEntry = NO;
        textField2.text = str2;
        [alert show];
        
        Button_Index = 4;
    }
    else
    {
        NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
        NSString *str = [user objectForKey:@"RIGHT_DOWN"];
        
        NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
        [sensor write:sensor.activePeripheral data:data];
    }
}

- (IBAction)ButtonRight_TouchUpInsid:(UIButton *)sender {
    if (!SettingState)
    {
        NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
        NSString *str = [user objectForKey:@"RIGHT_UP"];
        
        NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
        [sensor write:sensor.activePeripheral data:data];
    }
}

- (IBAction)ButtonRight_TouchUpOutside:(UIButton *)sender {
    if (!SettingState)
    {
        NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
        NSString *str = [user objectForKey:@"RIGHT_UP"];
        
        NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
        [sensor write:sensor.activePeripheral data:data];
    }
}

- (IBAction)ButtonLow_TouchDown:(UIButton *)sender {
    if (SettingState)
    {
        NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
        
        NSString *str1= [user objectForKey:@"LOW_DOWN"];

        UIAlertView *alert = [[UIAlertView alloc]
                              initWithTitle:NSLocalizedString(@"Setting_Title", @"")
                              message:NSLocalizedString(@"Setting_Text", @"")
                              delegate:self
                              cancelButtonTitle:NSLocalizedString(@"Setting_Cancel", @"")
                              otherButtonTitles:NSLocalizedString(@"Setting_OK", @"")
                              ,nil];
        
        alert.alertViewStyle = UIAlertViewStylePlainTextInput;
        
        UITextField *textField1 = [alert textFieldAtIndex:0];
        textField1.keyboardType = UIKeyboardTypeASCIICapable;
        textField1.placeholder = NSLocalizedString(@"Setting_Keydown", @"");
        textField1.text = str1;
        
        [alert show];
        
        Button_Index = 5;
    }
    else
    {
        NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
        NSString *str = [user objectForKey:@"LOW_DOWN"];
        
        NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
        [sensor write:sensor.activePeripheral data:data];
    }
}

- (IBAction)ButtonMedium_TouchDown:(UIButton *)sender {
    if (SettingState)
    {
        NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
        
        NSString *str1= [user objectForKey:@"MEDIUM_DOWN"];
        
        UIAlertView *alert = [[UIAlertView alloc]
                              initWithTitle:NSLocalizedString(@"Setting_Title", @"")
                              message:NSLocalizedString(@"Setting_Text", @"")
                              delegate:self
                              cancelButtonTitle:NSLocalizedString(@"Setting_Cancel", @"")
                              otherButtonTitles:NSLocalizedString(@"Setting_OK", @"")
                              ,nil];
        
        alert.alertViewStyle = UIAlertViewStylePlainTextInput;
        
        UITextField *textField1 = [alert textFieldAtIndex:0];
        textField1.keyboardType = UIKeyboardTypeASCIICapable;
        textField1.placeholder = NSLocalizedString(@"Setting_Keydown", @"");
        textField1.text = str1;
        
        [alert show];
        
        Button_Index = 6;
    }
    else
    {
        NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
        NSString *str = [user objectForKey:@"MEDIUM_DOWN"];
        
        NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
        [sensor write:sensor.activePeripheral data:data];
    }
}

- (IBAction)ButtonHigh_TouchDown:(UIButton *)sender {
    if (SettingState)
    {
        NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
        
        NSString *str1= [user objectForKey:@"HIGH_DOWN"];
        
        UIAlertView *alert = [[UIAlertView alloc]
                              initWithTitle:NSLocalizedString(@"Setting_Title", @"")
                              message:NSLocalizedString(@"Setting_Text", @"")
                              delegate:self
                              cancelButtonTitle:NSLocalizedString(@"Setting_Cancel", @"")
                              otherButtonTitles:NSLocalizedString(@"Setting_OK", @"")
                              ,nil];
        
        alert.alertViewStyle = UIAlertViewStylePlainTextInput;
        
        UITextField *textField1 = [alert textFieldAtIndex:0];
        textField1.keyboardType = UIKeyboardTypeASCIICapable;
        textField1.placeholder = NSLocalizedString(@"Setting_Keydown", @"");
        textField1.text = str1;
        
        [alert show];
        
        Button_Index = 7;
    }
    else
    {
        NSUserDefaults *user = [NSUserDefaults standardUserDefaults];
        NSString *str = [user objectForKey:@"HIGH_DOWN"];
        
        NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
        [sensor write:sensor.activePeripheral data:data];
    }
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/
#pragma mark - alertViewDelegate
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (buttonIndex == 1)
    {
        NSString * downString;
        NSString * upString;
        
        //保存数据
        UITextField *UITextField1=[alertView textFieldAtIndex:0];
        downString = UITextField1.text;
        
        if (Button_Index < 5)
        {
            UITextField *UITextField2=[alertView textFieldAtIndex:1];
            upString = UITextField2.text;
        }
        
        NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
        switch (Button_Index) {
            case 1:
                [userDefaults setObject:downString forKey:@"FORWARD_DOWN"];
                [userDefaults setObject:upString forKey:@"FORWARD_UP"];
                break;
            case 2:
                [userDefaults setObject:downString forKey:@"BACKWARD_DOWN"];
                [userDefaults setObject:upString forKey:@"BACKWARD_UP"];
                break;
            case 3:
                [userDefaults setObject:downString forKey:@"LEFT_DOWN"];
                [userDefaults setObject:upString forKey:@"LEFT_UP"];
                break;
            case 4:
                [userDefaults setObject:downString forKey:@"RIGHT_DOWN"];
                [userDefaults setObject:upString forKey:@"RIGHT_UP"];
                break;
            case 5:
                [userDefaults setObject:downString forKey:@"LOW_DOWN"];
                break;
            case 6:
                [userDefaults setObject:downString forKey:@"MEDIUM_DOWN"];
                break;
            case 7:
                [userDefaults setObject:downString forKey:@"HIGH_DOWN"];
                break;
            default:
                break;
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
        
            NSString *showSting = [NSString stringWithFormat:@"%@",[weatherDic objectForKey:@"State"]];
            
            if(![showSting isEqualToString:@"(null)"])
            {
                Label_State.text = showSting;
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
    UIBarButtonItem *rightBarButtonItem = [[UIBarButtonItem alloc]
                                           initWithTitle:NSLocalizedString(@"Button_Setting", @"")
                                           style:UIBarButtonItemStylePlain
                                           target:self
                                           action:@selector(doClickSettingAction)
                                           ];
    
    self.navigationItem.rightBarButtonItem = rightBarButtonItem;
    
    [Button_Forward setBackgroundImage:[UIImage imageNamed:@"button_forward_blue"] forState:UIControlStateNormal];
    Button_Forward.enabled = true;

    [Button_Backward setBackgroundImage:[UIImage imageNamed:@"button_backward_blue"] forState:UIControlStateNormal];
    Button_Backward.enabled = true;
    
    [Button_Left setBackgroundImage:[UIImage imageNamed:@"button_left_blue"] forState:UIControlStateNormal];
    Button_Left.enabled = true;
    
    [Button_Right setBackgroundImage:[UIImage imageNamed:@"button_right_blue"] forState:UIControlStateNormal];
    Button_Right.enabled = true;
    
    Button_Low.enabled = true;
    Button_Medium.enabled = true;
    Button_High.enabled = true;
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
