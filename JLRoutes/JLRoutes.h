/*
 Copyright (c) 2013, Joel Levin
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
 Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 Neither the name of JLRoutes nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  2015/12/7 梁立保 做了一些修改
 *   1.route和CanRoute的返回全部改为id (之前全部为BOOL)
 *   2.在每个路由block里返回任意非nil对象，表示不再继续走下一个路由  (之前返回YES则不再往下走)
 
 
 */

#import <Foundation/Foundation.h>


static NSString *const kJLRoutePatternKey = @"JLRoutePattern";
static NSString *const kJLRouteURLKey = @"JLRouteURL";
static NSString *const kJLRouteNamespaceKey = @"JLRouteNamespace";
static NSString *const kJLRouteWildcardComponentsKey = @"JLRouteWildcardComponents";
static NSString *const kJLRoutesGlobalNamespaceKey = @"JLRoutesGlobalNamespace";


@interface JLRoutes : NSObject
/** @class JLRoutes
 JLRoutes is a way to manage URL routes and invoke them from a URL.
 */

/// Returns the global routing namespace (this is used by the +addRoute methods by default)
+ (instancetype)globalRoutes;

/// Returns a routing namespace for the given scheme
+ (instancetype)routesForScheme:(NSString *)scheme;

/// Tells JLRoutes that it should manually replace '+' in parsed values to ' '. Defaults to YES.
+ (void)setShouldDecodePlusSymbols:(BOOL)shouldDeecode;
+ (BOOL)shouldDecodePlusSymbols;

/// Registers a routePattern with default priority (0) in the receiving scheme namespace.
+ (void)addRoute:(NSString *)routePattern handler:(id (^)(NSDictionary *parameters))handlerBlock;
- (void)addRoute:(NSString *)routePattern handler:(id (^)(NSDictionary *parameters))handlerBlock; // instance method

/// Registers multiple routePatterns for one handler with default priority (0) in the receiving scheme namespace.
+ (void)addRoutes:(NSArray *)routePatterns handler:(id (^)(NSDictionary *parameters))handlerBlock;
- (void)addRoutes:(NSArray *)routePatterns handler:(id (^)(NSDictionary *parameters))handlerBlock; // instance method


/// Removes a routePattern from the receiving scheme namespace.
+ (void)removeRoute:(NSString *)routePattern;
- (void)removeRoute:(NSString *)routePattern; // instance method

/// Removes all routes from the receiving scheme namespace.
+ (void)removeAllRoutes;
- (void)removeAllRoutes; // instance method

/// Unregister and delete an entire scheme namespace
+ (void)unregisterRouteScheme:(NSString *)scheme;

/// Registers a routePattern with default priority (0) using dictionary-style subscripting.
- (void)setObject:(id)handlerBlock forKeyedSubscript:(NSString *)routePatten;

/// Registers a routePattern in the global scheme namespace with a handlerBlock to call when the route pattern is matched by a URL.
/// The block returns a BOOL representing if the handlerBlock actually handled the route or not. If
/// a block returns NO, JLRoutes will continue trying to find a matching route.
+ (void)addRoute:(NSString *)routePattern priority:(NSUInteger)priority handler:(id (^)(NSDictionary *parameters))handlerBlock;
- (void)addRoute:(NSString *)routePattern priority:(NSUInteger)priority handler:(id (^)(NSDictionary *parameters))handlerBlock; // instance method

/// Routes a URL, calling handler blocks (for patterns that match URL) until one returns YES, optionally specifying add'l parameters
+ (id)routeURL:(NSURL *)URL;
+ (id)routeURL:(NSURL *)URL withParameters:(NSDictionary *)parameters;

- (id)routeURL:(NSURL *)URL; // instance method
- (id)routeURL:(NSURL *)URL withParameters:(NSDictionary *)parameters; // instance method

/// Returns whether a route exists for a URL
+ (BOOL)canRouteURL:(NSURL *)URL;
+ (BOOL)canRouteURL:(NSURL *)URL withParameters:(NSDictionary *)parameters;

- (BOOL)canRouteURL:(NSURL *)URL; // instance method
- (BOOL)canRouteURL:(NSURL *)URL withParameters:(NSDictionary *)parameters; // instance method

/// Prints the entire routing table
+ (NSString *)description;

/// Allows configuration of verbose logging. Default is NO. This is mostly just helpful with debugging.
+ (void)setVerboseLoggingEnabled:(BOOL)loggingEnabled;
+ (BOOL)isVerboseLoggingEnabled;

/// Controls whether or not this routes controller will try to match a URL with global routes if it can't be matched in the current namespace. Default is NO.
@property (nonatomic, assign) BOOL shouldFallbackToGlobalRoutes;

/// Called any time routeURL returns NO. Respects shouldFallbackToGlobalRoutes.
@property (nonatomic, copy) void (^unmatchedURLHandler)(JLRoutes *routes, NSURL *URL, NSDictionary *parameters);

@end
