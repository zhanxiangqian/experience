Python 2.4
import string
# 从字符串生成模板，其中标识符被$标记
new_style = string.Template('this is $thing')
# 给模板的substitute方法传入一个字典参数并调用之
print new_style.substitute({'thing':5})       # 输出：this is 5
print new_style.substitute({'thing':'test'}) # 输出：this is test
# 另外，也可以给substitute方法传递关键字参数
print new_style.substitute(thing=5)            # 输出：this is 5
print new_style.substitute(thing='test')      # 输出：this is test


Python 2.3中，用于标记—替换的字符串格式被写为更加繁琐的形式：
old_style = 'this is %(thing)s'
标识符被放在一对括弧中，括弧前面一个%，后面一个s。然后，还需要使用%操作符，使用的格式是将需要处理的字符串放在%操作符左边并在右边放上字典：

print old_style % {'thing':5}       # emits: this is 5
print old_style % {'thing':'test'} # emits: this is test