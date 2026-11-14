# deepseek_utils.py
"""DeepSeek API 简易封装（无面向对象，支持角色设定与上下文管理）"""

import os
from openai import OpenAI

# ---------- 模块级配置 ----------
_client = None
_role_prompt = "你是一个有用的助手"      # 默认角色
_history = []                           # 完整对话历史
MAX_HISTORY_TURNS = 10                  # 默认保留最近 10 轮（每轮 = 用户 + AI）


def _get_client():
    """惰性初始化 OpenAI 客户端，避免导入时即报错。"""
    global _client
    if _client is None:
        api_key = os.getenv("DS_API_KEY")
        if not api_key:
            raise ValueError("环境变量 DS_API_KEY 未设置")
        _client = OpenAI(api_key=api_key, base_url="https://ark.cn-beijing.volces.com/api/v3")
    return _client


def set_role(prompt: str):
    """设定 AI 的角色与行为准则。

    Args:
        prompt: 系统提示词，例如 "你是一名专业的 Python 工程师"
    """
    global _role_prompt
    _role_prompt = prompt


def set_max_history(turns: int):
    """设置上下文中保留的最大轮数（每轮 = 一条 user + 一条 assistant）。

    Args:
        turns: 正整数，表示保留最近多少轮对话
    """
    global MAX_HISTORY_TURNS
    if turns < 0:
        raise ValueError("turns 必须 >= 0")
    MAX_HISTORY_TURNS = turns


def clear_history():
    """清空当前会话的对话历史。"""
    global _history
    _history = []


def _build_messages() -> list:
    """构造发送给 API 的 messages 列表。

    规则：
      1. 首条为 system 消息，内容为当前角色提示词；
      2. 随后是截取后的历史消息（保留最近 MAX_HISTORY_TURNS 轮）；
      3. 历史已按 user / assistant 交替存储。
    """
    messages = [{"role": "system", "content": _role_prompt}]

    if MAX_HISTORY_TURNS == 0 or not _history:
        return messages

    # 截取最近 N 轮（每轮 2 条消息）
    keep = MAX_HISTORY_TURNS * 2
    recent = _history[-keep:] if len(_history) > keep else _history
    messages.extend(recent)
    return messages


def chat(user_input: str, temperature: float = 1.0) -> str:
    """发送一条用户消息，返回 AI 回复，并自动维护上下文。

    Args:
        user_input: 用户输入文本
        temperature: 采样温度（0~2），思考模式下不生效

    Returns:
        AI 回复的文本内容
    """
    client = _get_client()

    # 构造请求消息（system + 截取后的历史）
    messages = _build_messages()
    messages.append({"role": "user", "content": user_input})

    # 调用 API（非思考模式）
    response = client.chat.completions.create(
        model="deepseek-v4-1-flash-260910",
        messages=messages,
        stream=False,
        reasoning_effort="low",
        extra_body={"thinking": {"type": "disabled"}},  # 关闭思考模式
        temperature=temperature,
    )

    reply = response.choices[0].message.content

    # 将本轮问答追加到历史（保持 user / assistant 交替）
    _history.append({"role": "user", "content": user_input})
    _history.append({"role": "assistant", "content": reply})

    return reply


# ---------- 简单使用示例 ----------
if __name__ == "__main__":
    set_role("你是一名简洁、专业的助手，回答尽量简短")
    set_max_history(5)                     # 只保留最近 5 轮
    print(chat("你好"))
    print(chat("1+1等于几"))
    print(chat("刚才我问了什么"))
    print(chat("测试"))
    print(chat("刚才我问了什么"))

    print(chat("我最开始问的是什么"))
    print(chat("我最开始问的是什么"))