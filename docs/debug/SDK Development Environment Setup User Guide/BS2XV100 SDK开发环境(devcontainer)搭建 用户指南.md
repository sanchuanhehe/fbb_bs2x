# BS2XV100 SDK 开发环境（devcontainer）搭建 用户指南

本指南介绍如何通过 VSCode（支持 WSL、Linux 或 Windows (Docker)）、GitHub Workspace 以及 GitHub 模板仓库，快速搭建 BS2XV100 SDK 的开发环境。

---

## 1. 环境准备

### 1.1 安装 VSCode

- 访问 [VSCode 官网](https://code.visualstudio.com/) 下载并安装 VSCode。
- 推荐安装 Remote Development 插件包（包含 Remote - WSL、Remote - Containers、Remote - SSH）。
- 需要安装 [ms-vscode-remote.remote-containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) 插件。

### 1.2 安装 Docker

- Linux/Windows 用户请参考 [Docker 官网](https://www.docker.com/) 安装 Docker。
- WSL 用户需确保已安装 WSL2 并配置 Docker Desktop 支持 WSL2。

---

## 2. 获取开发环境代码

### 2.1 方法一: 使用 GitHub Workspace

1. 登录 [GitHub](https://github.com/)。
2. 访问 BS2XV100 SDK 的模板仓库（如 `https://github.com/sanchuanhehe/fbb_bs2x.git`）。
3. 点击 `Use this template`，选择`Open in a codespace`创建自己的 Workspace 仓库。

### 2.2 方法二: 本地开发

#### 2.2.1 克隆仓库到本地

在 VSCode 中打开终端，执行：

```bash
git clone https://github.com/sanchuanhehe/fbb_bs2x.git
cd fbb_bs2x
```

---

#### 2.2.2 使用 devcontainer 启动开发环境

##### 2.2.2.1 VSCode 打开项目

- 使用 VSCode 打开刚刚克隆的项目文件夹。

##### 2.2.2.2 启动 devcontainer

###### 方法一

- VSCode 会自动检测 `.devcontainer` 文件夹，提示“在容器中重新打开”。
- 点击“在容器中重新打开”，VSCode 会自动拉取并构建开发环境镜像。

> **注意**：首次构建 devcontainer 可能需要几分钟时间，具体取决于网络和镜像大小。

###### 方法二

- 使用`ctrl+shift+p` 打开命令面板，输入 `Remote-Containers: Reopen in Container`，选择该命令。
- VSCode 会自动拉取并构建开发环境镜像。
- 构建完成后，VSCode 会在 devcontainer 中打开项目。

---

## 4. 其他平台说明

### 4.1 WSL 下使用

- 在 WSL 终端中克隆仓库并用 VSCode 打开，流程同上。

### 4.2 Linux 原生环境

- 直接在 Linux 终端操作，流程同上。

### 4.3 Windows (Docker) 环境

- 确保 Docker Desktop 已启动，流程同上。

---

## 5. 常见问题

- **devcontainer 启动失败**：请检查 Docker 是否正常运行，或尝试重启 VSCode。
- **依赖安装失败**：请检查网络连接，或更换镜像源。

---

## 6. 参考资料

- [VSCode Remote Development 官方文档](https://code.visualstudio.com/docs/remote/remote-overview)
- [GitHub Template 仓库说明](https://docs.github.com/en/repositories/creating-and-managing-repositories/creating-a-template-repository)
- [Docker 官方文档](https://docs.docker.com/)

---
