//
//  ViewController.m
//  KVC-KVO
//
//  Created by 苏沫离 on 2020/11/17.
//

#import "ViewController.h"
#import "Model.h"


@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.view.backgroundColor = UIColor.whiteColor;
    
    NSLog(@"NSAverageKeyValueOperator === %@",NSAverageKeyValueOperator);
    
    [self test];
}



- (void)test{
    
    BankAccount *model = [[BankAccount alloc] init];
//    [model setValue:@[@"1",@"2"] forKey:@"array"];
//    [model setValue:@"字符串" forKey:@"string"];
//    [model setValue:@(YES) forKey:@"show"];
    
    
    NSMutableArray;
}

@end
