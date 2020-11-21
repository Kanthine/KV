//
//  Model.m
//  KVC-KVO
//
//  Created by 苏沫离 on 2020/11/17.
//

#import "Model.h"


@implementation Person

@end



@implementation Transaction

@end



@implementation BankAccount


#pragma mark - 定义 集合 的方法

/** -countOf<Key>
 */
- (NSUInteger)countOfTransactions{
    return self.transactions.count;
}

/** -objectIn<Key>AtIndex: 或者 -<key>AtIndexes:  只需实现其中之一
 */
- (id)objectInTransactionsAtIndex:(NSUInteger)index{
    return [self.transactions objectAtIndex:index];
}

- (NSArray *)transactionsAtIndexes:(NSIndexSet *)indexes{
    return [self.transactions objectsAtIndexes:indexes];
}

/** -get<Key>:range: 此方法是可选的，但可以提高性能。
 * 它从集合中返回属于指定范围内并与NSArraymethod相对应的对象getObjects:range:。Transactions数组的实现是：
 */
- (void)getTransactions:(Transaction * __unsafe_unretained *)buffer range:(NSRange)inRange{
    [self.transactions getObjects:buffer range:inRange];
}

- (void)insertObject:(Transaction *)object inTransactionsAtIndex:(NSUInteger)index{
    [self.transactions insertObject:object atIndex:index];
}

- (void)insertTransactions:(NSArray *)array atIndexes:(NSIndexSet *)indexes{
    [self.transactions insertObjects:array atIndexes:indexes];
}

- (void)removeObjectFromTransactionsAtIndex:(NSUInteger)index{
    [self.transactions removeObjectAtIndex:index];
}

- (void)removeTransactionsAtIndexes:(NSIndexSet *)indexes{
    [self.transactions removeObjectsAtIndexes:indexes];
}

- (void)replaceObjectInTransactionsAtIndex:(NSUInteger)index withObject:(id)object{
    [self.transactions replaceObjectAtIndex:index withObject:object];
}

- (void)replaceTransactionsAtIndexes:(NSIndexSet *)indexes withTransactions:(NSArray *)array{
    [self.transactions replaceObjectsAtIndexes:indexes withObjects:array];
}

@end
