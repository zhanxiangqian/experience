# -*- coding:utf-8 -*-
class GrandPa():
    def __init__(self):
        print('I\'m GrandPa')


class Father(GrandPa):
    def __init__(self):
        print('I\'m Father!')

class Son(Father):
    """A simple example class"""
    i = 12345
    def __init__(self):
        print(u'这是构造函数,son')
    def sayHello(self):
        return 'hello world'
import chardet

if __name__ == '__main__':
    son = Son()
    aa = 'abcd我' #没有加u 转换为Unicode对象
    print aa
    print chardet.detect(aa)
    # 类型帮助信息 
    chinese = '类型帮助信息: '
    print chinese
    print chardet.detect(chinese)
    print u'类型帮助信息: ', Son.__doc__
    #类型所继承的基类
    print u'类型所继承的基类:',Son.__bases__
    print u'GrandPa类型所继承的基类:',GrandPa.__bases__
    #类型字典
    print u'类型字典:',Son.__dict__
    #类型所在模块
    print u'类型所在模块:',Son.__module__
    #类型名称
    print u"类型名称", Son.__name__
    #实例类型
    print u'类型:',GrandPa().__class__    
    print u'父类型:',GrandPa.__bases__    
    print u'类型:',isinstance(GrandPa(),object)
    print u'type类型:',type(GrandPa())
    aa = [[11,12,12],[21,22,23],[31,32,33]]
    aa[0][1] &= 1
    
    bb = [11,12,13]
    bb[0] = bb[0] & 0
    print aa
    print bb