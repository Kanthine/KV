//
//  YLKeyValueCoding.h
//  键-值编码 : KVC
//
//  Created by 苏沫离 on 2020/11/16.
//  Copyright © 2020 苏沫离. All rights reserved.
//


#import <Foundation/Foundation.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSDictionary.h>
#import <Foundation/NSOrderedSet.h>
#import <Foundation/NSSet.h>
#import <Foundation/NSException.h>


// https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/KeyValueCoding/index.html#//apple_ref/doc/uid/10000107-SW1

//https://www.jianshu.com/p/45cbd324ea65






@class NSError, NSString;

NS_ASSUME_NONNULL_BEGIN

/** KVC 操作失败时引发的异常。异常的 userInfo 字典将至少包含两个内容:
 *  - NSTargetObjectUserInfoKey : KVC 消息的接收者
 *  - NSUnknownUserInfoKey : KVC 消息中使用的 Key
 */
FOUNDATION_EXPORT NSExceptionName const NSUndefinedKeyException;

typedef NSString * NSKeyValueOperator NS_STRING_ENUM;

/** 使用键值编码时，可以在键路径中嵌入的运算符：
 * 集合运算符是一小部分关键字之一，其后跟一个 @ 符号，该符号指定 getter 在返回数据之前应执行的操作以某种方式处理数据。
 */
FOUNDATION_EXPORT NSKeyValueOperator const NSAverageKeyValueOperator;
FOUNDATION_EXPORT NSKeyValueOperator const NSCountKeyValueOperator;
FOUNDATION_EXPORT NSKeyValueOperator const NSDistinctUnionOfArraysKeyValueOperator;
FOUNDATION_EXPORT NSKeyValueOperator const NSDistinctUnionOfObjectsKeyValueOperator;
FOUNDATION_EXPORT NSKeyValueOperator const NSDistinctUnionOfSetsKeyValueOperator;
FOUNDATION_EXPORT NSKeyValueOperator const NSMaximumKeyValueOperator;
FOUNDATION_EXPORT NSKeyValueOperator const NSMinimumKeyValueOperator;
FOUNDATION_EXPORT NSKeyValueOperator const NSSumKeyValueOperator;
FOUNDATION_EXPORT NSKeyValueOperator const NSUnionOfArraysKeyValueOperator;
FOUNDATION_EXPORT NSKeyValueOperator const NSUnionOfObjectsKeyValueOperator;
FOUNDATION_EXPORT NSKeyValueOperator const NSUnionOfSetsKeyValueOperator;

@interface NSObject(YLKeyValueCoding)

/** 默认返回YES，表示如果没有找到 setter 方法的话，会按照 _key，_iskey，key，iskey的顺序搜索成员，设置成NO就不这样搜索
 *   -valueForKey:
 *   -setValue:forKey:
 *   -mutableArrayValueForKey:
 *   -storedValueForKey:
 *   -takeStoredValue:forKey:
 *   -takeValue:forKey:
*/
@property (class, readonly) BOOL accessInstanceVariablesDirectly;

/** 根据指定的 key 键，获取属性值或相关对象；
 * 给定一个‘一对多’关系的键，返回一个不可变数组或一个包含所有相关对象的不可变集合。
 *
 * 这个方法的默认实现如下所示:
 * 1、首先按 -get<Key>， -<Key>，或 -is<Key>，的顺序方法查找 getter 方法，如果找到，就调用它：
 *    如果获取的结果是 BOOL 或者 CGRect 等值类型，转为 NSNumber 或 NSValue；
 * 2、如果上面的 getter 方法没有找到，针对集合 NSArray ：
 *     KVC 会查找 -countOf<Key> 和 -indexIn<Key>OfObject: 和 -objectIn<Key>AtIndex: 格式的方法。
 *    （对于NSOrderedSet类的 -objectsAtIndexes: 方法）
 * 如果找到 -countOf<Key> 和另两个方法中的一个，则返回一个可以响应 NSArray 所有方法的代理集合；
 * 3、如果上面的方法没有找到，那么会同时查找 -countOf<Key>，enumeratorOf<Key>,memberOf<Key>格式的方法。
 *    如果这三个方法都找到，那么就返回一个可以响应NSSet所的方法的代理集合；
 *    和上面一样，给这个代理集合发NSSet的消息，就会以countOf<Key>，enumeratorOf<Key>,memberOf<Key>组合的形式调用。
 * 4、上述步骤都没找到，如果对象所属的类属性 +accessinstancevariablesdirect 返回 YES，
 *    则搜索 _<key>, _is<Key>, <key>, 或 is<Key> 的实例变量；
 *    找到这样的实例变量就返回该实例变量的值，与步骤1中对NSNumber或NSValue的转换类型相同；
 * 5、 如果上述步骤都没找到，调用 -valueForUndefinedKey: 并返回结果：
 *     -valueForUndefinedKey: 方法默认引发异常 NSUndefinedKeyException，我们需要重写该方法！

注意兼容性:
 * - 对于兼容 向后二进制，在步骤1和3之间搜索一个名称匹配模式 -_get<Key> 或 -_<Key> 的访问器方法。如果找到这样的方法，就调用它，与步骤1中对NSNumber或NSValue的转换类型相同。名字以下划线开头的KVC访问方法在Mac OS 10.3中已经被弃用了。
 * - 步骤5中描述的行为是对Mac OS 10.2的一个改变，在10.2中，实例变量搜索顺序是<key>， _<key>。
 * -对于兼容 向后二进制，如果 -handleQueryWithUnboundKey: 在目标类实现，在第6步 -handleQueryWithUnboundKey: 将被调用替代 -valueForUndefinedKey:；
*/
- (nullable id)valueForKey:(NSString *)key;

/** 根据指定的 key ，设置 value
 * 默认实现如下:
 * 1、首先搜索 -setter 方法：如果找到这样的方法，就检查其参数的类型。
 *   如果值为 nil，则调用 -setNilValueForKey:
 *   如果值为 BOOL 或者 CGRect 等值类型，转为 NSNumber 或 NSValue；
 * 2、没有找到 -setter 方法；
 *   如果该类的属性 +accessinstancevariablesdirect 返回YES，则搜索名称为 _<key>, _is<Key>, <key>, 或 is<Key> 的实例变量；
 *   如果找到了这样的实例变量，则赋值；
 * 3、如果上面列出的方法或者成员变量都不存在，系统将会调用该对象的 -setValue:forUndefinedKey: 方法，默认是抛出异常。
 */
- (void)setValue:(nullable id)value forKey:(NSString *)key;

/** KVC的正确性验证：下述方法用来验证 key 对应的 Value 是否可用
 * 用来检查 set 的值是否正确，为不正确的值做一个替换值或者拒绝设置新值并返回错误原因。
 * 默认返回 YES ！如果开发者需要验证能不能用 KVC 设定某个值时，需要重写该方法，然后 KVC 调用这个方法来验证！
 * @注意 KVC在设值时不会主动去做验证，需要开发者手动去验证。所以即使你在类里面写了验证方法，但是KVC因为不会去主动验证，所以还是能够设值成功。
 */
- (BOOL)validateValue:(inout id _Nullable * _Nonnull)ioValue forKey:(NSString *)inKey error:(out NSError **)outError;

/** 对象的属性可以是一对一的，也可以是一对多的。一对多的属性要么是有序的(数组)，要么是无序的(集合)。
 *  不可变的有序容器属性(NSArray)和无序容器属性(NSSet)一般可以使用 -valueForKey: 来获取
 *  KVC 使用了一种更灵活的方式来管理容器类，下述方法返回一个可变的数组 NSMutableArray
 *
 * KVC的搜索顺序如下
 * 1、搜索下述格式的方法:
 *     -insertObject:in<Key>AtIndex:
 *     -removeObjectFrom<Key>AtIndex:
 *     -[NSMutableArray insertObjects:atIndexes:]
 *     -[NSMutableArray removeObjectsAtIndexes:]
 *  如果至少找到一个 -insert 方法和一个 -remove 方法，那么返回一个 NSKeyValueFastMutableArray2
 * 2、如果上步的方法没有找到，则搜索 -set<Key>: 格式的方法；
 *    如果找到，那么发送给代理集合的NSMutableArray最终都会调用set<Key>:方法。
 *    也就是说，-mutableArrayValueForKey: 取出的代理集合修改后，用 -set<Key>: 重新赋值回去去。
 *    性能问题：这样做效率会低很多；所以推荐实现上面的方法。
 * 3、如果上一步的方法还还没有找到，再检查类方法+ (BOOL)accessInstanceVariablesDirectly,如果返回YES
 *    按_<key>，<key>的顺序搜索成员变量名，如果找到，那么发送的 NSMutableArray 消息方法直接交给这个成员变量处理。
 * 4、如果还是找不到，则调用 -valueForUndefinedKey:
 */
- (NSMutableArray *)mutableArrayValueForKey:(NSString *)key;

/* Given a key that identifies an _ordered_ and uniquing to-many relationship, return a mutable ordered set that provides read-write access to the related objects. Objects added to the mutable ordered set will become related to the receiver, and objects removed from the mutable ordered set will become unrelated.

The default implementation of this method recognizes the same simple accessor methods and ordered set accessor methods as -valueForKey:'s, and follows the same direct instance variable access policies, but always returns a mutable collection proxy object instead of the immutable collection that -valueForKey: would return. It also:
    1. Searches the class of the receiver for methods whose names match the patterns -insertObject:in<Key>AtIndex: and -removeObjectFrom<Key>AtIndex: (corresponding to the two most primitive methods defined by the NSMutableOrderedSet class), and also -insert<Key>:atIndexes: and -remove<Key>AtIndexes: (corresponding to -[NSMutableOrderedSet insertObjects:atIndexes:] and -[NSMutableOrderedSet removeObjectsAtIndexes:). If at least one insertion method and at least one removal method are found each NSMutableOrderedSet message sent to the collection proxy object will result in some combination of -insertObject:in<Key>AtIndex:, -removeObjectFrom<Key>AtIndex:, -insert<Key>:atIndexes:, and -remove<Key>AtIndexes: messages being sent to the original receiver of -mutableOrderedSetValueForKey:. If the class of the receiver also implements an optional method whose name matches the pattern -replaceObjectIn<Key>AtIndex:withObject: or -replace<Key>AtIndexes:with<Key>: that method will be used when appropriate for best performance.
    2. Otherwise (no set of ordered set mutation methods is found), searches the class of the receiver for an accessor method whose name matches the pattern -set<Key>:. If such a method is found each NSMutableOrderedSet message sent to the collection proxy object will result in a -set<Key>: message being sent to the original receiver of -mutableOrderedSetValueForKey:.
    3. Otherwise (no set of ordered set mutation methods or simple accessor method is found), if the receiver's class' +accessInstanceVariablesDirectly property returns YES, searches the class of the receiver for an instance variable whose name matches the pattern _<key> or <key>, in that order. If such an instance variable is found, each NSMutableOrderedSet message sent to the collection proxy object will be forwarded to the instance variable's value, which therefore must typically be an instance of NSMutableOrderedSet or a subclass of NSMutableOrderedSet.
    4. Otherwise (no set of ordered set mutation methods, simple accessor method, or instance variable is found), returns a mutable collection proxy object anyway. Each NSMutableOrderedSet message sent to the collection proxy object will result in a -setValue:forUndefinedKey: message being sent to the original receiver of -mutableOrderedSetValueForKey:. The default implementation of -setValue:forUndefinedKey: raises an NSUndefinedKeyException, but you can override it in your application.

Performance note: the repetitive -set<Key>: messages implied by step 2's description are a potential performance problem. For better performance implement insertion and removal methods that fulfill the requirements for step 1 in your KVC-compliant class. For best performance implement a replacement method too.
*/
- (NSMutableOrderedSet *)mutableOrderedSetValueForKey:(NSString *)key API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

/** 返回一个可变的无序数组，
 *  KVC的搜索顺序如下：
 *

The default implementation of this method recognizes the same simple accessor methods and set accessor methods as -valueForKey:'s, and follows the same direct instance variable access policies, but always returns a mutable collection proxy object instead of the immutable collection that -valueForKey: would return. It also:
    1. Searches the class of the receiver for methods whose names match the patterns -add<Key>Object: and -remove<Key>Object: (corresponding to the two primitive methods defined by the NSMutableSet class) and also -add<Key>: and -remove<Key>: (corresponding to -[NSMutableSet unionSet:] and -[NSMutableSet minusSet:]). If at least one addition method and at least one removal method are found each NSMutableSet message sent to the collection proxy object will result in some combination of -add<Key>Object:, -remove<Key>Object:, -add<Key>:, and -remove<Key>: messages being sent to the original receiver of -mutableSetValueForKey:. If the class of the receiver also implements an optional method whose name matches the pattern -intersect<Key>: or -set<Key>: that method will be used when appropriate for best performance.
    2. Otherwise (no set of set mutation methods is found), searches the class of the receiver for an accessor method whose name matches the pattern -set<Key>:. If such a method is found each NSMutableSet message sent to the collection proxy object will result in a -set<Key>: message being sent to the original receiver of -mutableSetValueForKey:.
    3. Otherwise (no set of set mutation methods or simple accessor method is found), if the receiver's class' +accessInstanceVariablesDirectly property returns YES, searches the class of the receiver for an instance variable whose name matches the pattern _<key> or <key>, in that order. If such an instance variable is found, each NSMutableSet message sent to the collection proxy object will be forwarded to the instance variable's value, which therefore must typically be an instance of NSMutableSet or a subclass of NSMutableSet.
    4. Otherwise (no set of set mutation methods, simple accessor method, or instance variable is found), returns a mutable collection proxy object anyway. Each NSMutableSet message sent to the collection proxy object will result in a -setValue:forUndefinedKey: message being sent to the original receiver of -mutableSetValueForKey:. The default implementation of -setValue:forUndefinedKey: raises an NSUndefinedKeyException, but you can override it in your application.

Performance note: the repetitive -set<Key>: messages implied by step 2's description are a potential performance problem. For better performance implement methods that fulfill the requirements for step 1 in your KVC-compliant class.
*/
- (NSMutableSet *)mutableSetValueForKey:(NSString *)key;

/** 使用该方法访问多层嵌套是比较方便的
*/
- (nullable id)valueForKeyPath:(NSString *)keyPath;
- (void)setValue:(nullable id)value forKeyPath:(NSString *)keyPath;

- (BOOL)validateValue:(inout id _Nullable * _Nonnull)ioValue forKeyPath:(NSString *)inKeyPath error:(out NSError **)outError;


- (NSMutableArray *)mutableArrayValueForKeyPath:(NSString *)keyPath;
- (NSMutableOrderedSet *)mutableOrderedSetValueForKeyPath:(NSString *)keyPath API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));
- (NSMutableSet *)mutableSetValueForKeyPath:(NSString *)keyPath;


/** 如果调用 -valueForKey: 不能识别 key 值，系统默认使用下述方法处理；
 * 如果不想引发异常 NSUndefinedKeyException，需要重写下述方法
 */
- (nullable id)valueForUndefinedKey:(NSString *)key;

/** 如果调用 -setValue:forKey: 不能识别 key 值，系统默认使用下述方法处理；
 * 如果不想引发异常 NSUndefinedKeyException，需要重写下述方法
 */
- (void)setValue:(nullable id)value forUndefinedKey:(NSString *)key;

/** 在默认实现中，将 BOOL、CGRect 类型属性设置为 nil 值时，会调用  -setNilValueForKey: 方法
 * 默认引发异常 NSInvalidArgumentException；需要重写下述方法
 */
- (void)setNilValueForKey:(NSString *)key;

/** 为数组 keys 中的每个键调用 -valueForKey:
 * @param keys 指定的键的集合
 * @return 返回的 NSDictionary 包含数组 keys 中所有键的值
 * @note -valueForKey: 返回 nil 的条目在字典中以 NSNull 作为其值
 */
- (NSDictionary<NSString *, id> *)dictionaryWithValuesForKeys:(NSArray<NSString *> *)keys;

/** 键值编码：使用指定字典 keyedValues 中的值设置接收方的属性
 * 默认实现为每个键值对调用 -setValue:forKey:
 * 根据需要用 nil 替换 NSNull 对象：值为 NSNull 的条目会导致 -setValue:nil forKey:key 消息被发送到接收者
 */
- (void)setValuesForKeysWithDictionary:(NSDictionary<NSString *, id> *)keyedValues;

@end

@interface NSArray<ObjectType>(NSKeyValueCoding)

/** 返回一个数组，该数组是对每个元素调用 -valueForKey: 的结果集
 * 如果某个元素调用 -valueForKey: 返回nil，则该数组使用 NSNull ；
 */
- (id)valueForKey:(NSString *)key;

/** 数组的每个元素上调用 -setValue:forKey:
 */
- (void)setValue:(nullable id)value forKey:(NSString *)key;

@end

@interface NSDictionary<KeyType, ObjectType>(NSKeyValueCoding)

/** 使用 -objectForKey: 的返回结果
 */
- (nullable ObjectType)valueForKey:(NSString *)key;

@end

@interface NSMutableDictionary<KeyType, ObjectType>(NSKeyValueCoding)

/** 调用 -setObject:forKey: 存储，
 * @param value 如果为 nil，则调用 -removeObjectForKey:
 */
- (void)setValue:(nullable ObjectType)value forKey:(NSString *)key;

@end

@interface NSOrderedSet<ObjectType>(NSKeyValueCoding)

/* Return an ordered set containing the results of invoking -valueForKey: on each of the receiver's members. The returned ordered set might not have the same number of members as the receiver. The returned ordered set will not contain any elements corresponding to instances of -valueForKey: returning nil, nor will it contain duplicates.
*/
- (id)valueForKey:(NSString *)key API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

/** 对每个成员调用 -setValue:forKey:
 */
- (void)setValue:(nullable id)value forKey:(NSString *)key API_AVAILABLE(macos(10.7), ios(5.0), watchos(2.0), tvos(9.0));

@end

@interface NSSet<ObjectType>(NSKeyValueCoding)

/** 返回一个集合，该集合包含调用-valueForKey的结果:在每个receiver成员上。返回的集合的成员数可能与接收方的成员数不同。返回的集合将不包含任何与-valueForKey实例对应的元素:返回nil(与-[NSArray(NSKeyValueCoding) valueForKey:]相反，它可能会把NSNulls放在它返回的数组中)。
 * Return a set containing the results of invoking -valueForKey: on each of the receiver's members. The returned set might not have the same number of members as the receiver. The returned set will not contain any elements corresponding to instances of -valueForKey: returning nil (in contrast with -[NSArray(NSKeyValueCoding) valueForKey:], which may put NSNulls in the arrays it returns).
*/
- (id)valueForKey:(NSString *)key;

/** 对每个成员调用 -setValue:forKey:
 */
- (void)setValue:(nullable id)value forKey:(NSString *)key;

@end

NS_ASSUME_NONNULL_END


/**
* 使用符合键值编码的对象：NSObject 的子类，均可使用键值编码：KVC
*  <1>、访问对象的属性；如 getter 的 -valueForKey: 方法和 setter 的 -setValue:forKey: 方法；
 通过键访问对象的属性，使用键来定位底层数据并与之交互；
*  <2>、操作集合类型的属性：如 NSArray 对象，就像访问其他任何属性一样；
 为提升性能，允许开发者通过标准化接口使用自定义集合对象，
*  <3>、在集合类型的对象上使用运算符：向 key 键中插入运算符，然后返回一个新的集合；
*  <4>、访问基本数据类型的属性：如 BOOL、CGRect 等，默认转为 NSNumber 、NSValue；
*  <5>、通过键路径访问属性：针对多层嵌套，使用键路径可以很方便的访问某个值

* Cocoa 中依赖于键值编码的技术：
*   <1>、键值观察 KVC：该机制使对象能够注册由另一个对象的属性更改驱动的异步通知。
*   <2>、Cocoa bindings： 提供了一种增强应用程序功能和一致性的方法，同时减少了开发者必须编写和维护的代码量
 绑定是一个对象的属性，可以将其绑定到另一个对象的属性，从而使一个对象的更改反映在另一个对象中。
 例如，一个文本字段的“值”绑定可能绑定到特定模型对象的温度属性。
 更典型地，一个绑定可能指定一个控制器对象“呈现”一个模型对象，而另一个绑定可能指定一个文本字段的值绑定到控制器所呈现的对象的温度属性。
*   <3>、CoreData： 为对象生命周期和对象图管理（包括持久性）相关的常见任务提供了通用的自动化解决方案。
*   <4>、AppleScript：该脚本语言可以直接控制可编写脚本的应用程序以及macOS的许多部分；
 Cocoa 的脚本使用键值编码来获取和设置脚本对象中的信息
 */

