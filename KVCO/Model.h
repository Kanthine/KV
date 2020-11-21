//
//  Model.h
//  KVC-KVO
//
//  Created by 苏沫离 on 2020/11/17.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface Person : NSObject

@end



@interface Transaction : NSObject
 
@property (nonatomic ,strong) NSString* payee;   // To whom
@property (nonatomic ,strong) NSNumber* amount;  // How much
@property (nonatomic ,strong) NSDate* date;      // When
 
@end



@interface BankAccount : NSObject

@property (nonatomic ,assign) NSNumber* currentBalance;              // An attribute
@property (nonatomic ,strong) Person* owner;                         // A to-one relation
@property (nonatomic ,strong) NSMutableArray<Transaction* >* transactions; // A to-many relation

@end

NS_ASSUME_NONNULL_END
