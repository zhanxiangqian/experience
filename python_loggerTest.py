# -*- coding: utf-8 -*-
import logging

# python官方文档中提供的一段示例，使用logging模块产生logger对象
FORMAT = "%(levelname)-8s [%(name)s] : %(message)s"
logging.basicConfig(format = FORMAT)
# 创建一个日志对象
logger = logging.getLogger('tcpserver')
logger.warning('Protocol problem: %s', 'connection reset')
# 设置级别
logger.setLevel(logging.DEBUG)   
