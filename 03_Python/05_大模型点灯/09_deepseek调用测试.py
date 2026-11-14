# 请先安装 OpenAI SDK：pip install openai
import os
from openai import OpenAI

# ==================== 1. 初始化 DeepSeek API 客户端 ====================
# 从环境变量读取API密钥，避免硬编码泄露风险
api_key = os.getenv("DS_API_KEY")
if not api_key:
    raise ValueError("环境变量 DS_API_KEY 未设置")

# 创建OpenAI兼容客户端，指向DeepSeek官方API端点
client = OpenAI(
    api_key=api_key,
    base_url="https://ark.cn-beijing.volces.com/api/v3"  # DeepSeek兼容OpenAI接口协议
)

# ==================== 2. 发送对话请求 ====================
response = client.chat.completions.create(
    model="glm-4-7-251222",          # 模型名称：flash为轻量快速版本
    messages=[
        {"role": "system", "content": "你是一个有用的助手"},  # 系统提示词：设定AI角色与行为准则
        {"role": "user", "content": "你好"},                  # 用户消息：实际输入内容
    ],
    stream=False,                    # False=等待完整响应后一次性返回；True=逐token流式输出
    reasoning_effort="low",         # 推理强度：控制模型思考深度（low/medium/high）
    extra_body={                     # DeepSeek专属扩展参数（非OpenAI标准字段）
        "thinking": {"type": "disabled"}  # 关闭思维链输出：不返回中间推理过程，仅返回最终答案
    }
)

# ==================== 3. 提取并打印回复内容 ====================
print(response.choices[0].message.content)  # choices[0]取第一条回复，.content获取纯文本内容