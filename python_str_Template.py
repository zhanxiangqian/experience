Python 2.4
import string
# ���ַ�������ģ�壬���б�ʶ����$���
new_style = string.Template('this is $thing')
# ��ģ���substitute��������һ���ֵ����������֮
print new_style.substitute({'thing':5})       # �����this is 5
print new_style.substitute({'thing':'test'}) # �����this is test
# ���⣬Ҳ���Ը�substitute�������ݹؼ��ֲ���
print new_style.substitute(thing=5)            # �����this is 5
print new_style.substitute(thing='test')      # �����this is test


Python 2.3�У����ڱ�ǡ��滻���ַ�����ʽ��дΪ���ӷ�������ʽ��
old_style = 'this is %(thing)s'
��ʶ��������һ�������У�����ǰ��һ��%������һ��s��Ȼ�󣬻���Ҫʹ��%��������ʹ�õĸ�ʽ�ǽ���Ҫ������ַ�������%��������߲����ұ߷����ֵ䣺

print old_style % {'thing':5}       # emits: this is 5
print old_style % {'thing':'test'} # emits: this is test