//
//  AppDelegate.m
//  KVC-KVO
//
//  Created by 苏沫离 on 2020/11/17.
//

#import "AppDelegate.h"
#import "ViewController.h"

@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    
    self.window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];
    [self.window makeKeyAndVisible];
    
    ViewController *mainVC = [[ViewController alloc] init];
    self.window.rootViewController = mainVC;
    return YES;
}

@end
