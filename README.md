# simpleGameGateWay

## 一个简单的单线程Reactor网络框架下的game gate

封装来自client端的socket， 和上游服务器端socket。  

中间封装一个proxy，实现fake login达到客户端不断开的情况下， 重连上游游戏服务。  
proxy的转发颗粒度为单个游戏逻辑包， 而非无脑socket stream流。
