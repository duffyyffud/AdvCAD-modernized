# AdvCAD ChatBot Web Application - Design Plan with MCP
# AdvCAD チャットボット基本設計書 (MCP対応版)

## 1. System Overview with MCP / MCPを含むシステム概要

### Purpose / 目的
MCP (Model Context Protocol) を活用した自然言語3D CADモデル生成システム
- MCPサーバーとしてAdvCAD機能を公開
- 複数のAIアシスタントから利用可能
- 標準化されたツールインターフェース

### MCP Architecture Benefits
1. **Standardized Interface**: Any MCP-compatible AI can use AdvCAD
2. **Tool Composition**: Combine with other MCP tools (file system, databases)
3. **Context Sharing**: Maintain model state across conversations
4. **Multi-Agent Ready**: Different agents can collaborate on same model

## 2. MCP Server Implementation / MCPサーバー実装

### MCP Server Structure
```
advcad-mcp-server/
├── server.py          # MCP server implementation
├── tools.json         # Tool definitions
├── package.json       # Node.js wrapper (if needed)
└── handlers/
    ├── generate.py    # Model generation
    ├── validate.py    # Validation
    ├── mesh.py        # Mesh optimization
    ├── diagnose.py    # CSG diagnostics
    └── visualize.py   # Visualization
```

### MCP Tool Definitions
```json
{
  "tools": [
    {
      "name": "generate_gm3d",
      "description": "自然言語の説明から.gm3dファイルを生成",
      "parameters": {
        "description": {
          "type": "string",
          "description": "3Dモデルの日本語説明"
        },
        "track_operations": {
          "type": "boolean",
          "description": "CSG操作を追跡するか",
          "default": true
        }
      }
    },
    {
      "name": "validate_gm3d",
      "description": ".gm3dファイルの妥当性を検証",
      "parameters": {
        "content": {
          "type": "string",
          "description": ".gm3dファイルの内容"
        }
      }
    },
    {
      "name": "optimize_mesh",
      "description": "自動メッシュサイズ最適化を実行",
      "parameters": {
        "gm3d_path": {
          "type": "string",
          "description": ".gm3dファイルのパス"
        }
      }
    },
    {
      "name": "diagnose_csg_failure",
      "description": "CSG操作の失敗を診断",
      "parameters": {
        "gm3d_content": {
          "type": "string",
          "description": "失敗した.gm3dコンテンツ"
        },
        "operations": {
          "type": "array",
          "description": "CSG操作のリスト"
        }
      }
    },
    {
      "name": "interpret_error",
      "description": "技術的エラーを日本語で解釈",
      "parameters": {
        "error_log": {
          "type": "string",
          "description": "AdvCADエラーログ"
        },
        "context": {
          "type": "object",
          "description": "エラーコンテキスト"
        }
      }
    }
  ]
}
```

## 3. MCP Server Implementation / MCPサーバー実装

### server.py
```python
import asyncio
import json
from typing import Any, Dict
from mcp.server import MCPServer, Tool, ToolResult
from handlers import (
    generate_handler,
    validate_handler,
    mesh_handler,
    diagnose_handler,
    visualize_handler
)

class AdvCADMCPServer(MCPServer):
    def __init__(self):
        super().__init__("advcad-mcp")
        self.register_tools()
        
    def register_tools(self):
        self.add_tool(Tool(
            name="generate_gm3d",
            description="Generate .gm3d from natural language",
            handler=self.handle_generate
        ))
        
        self.add_tool(Tool(
            name="validate_gm3d",
            description="Validate .gm3d file",
            handler=self.handle_validate
        ))
        
        self.add_tool(Tool(
            name="optimize_mesh",
            description="Auto-optimize mesh generation",
            handler=self.handle_mesh
        ))
        
        self.add_tool(Tool(
            name="diagnose_csg_failure",
            description="Diagnose CSG operation failures",
            handler=self.handle_diagnose
        ))
    
    async def handle_generate(self, params: Dict[str, Any]) -> ToolResult:
        """自然言語から.gm3d生成"""
        description = params.get("description")
        track_ops = params.get("track_operations", True)
        
        result = await generate_handler.process(description, track_ops)
        
        return ToolResult(
            success=result["success"],
            data=result
        )
    
    async def handle_validate(self, params: Dict[str, Any]) -> ToolResult:
        """gm3dファイル検証"""
        content = params.get("content")
        
        result = await validate_handler.process(content)
        
        return ToolResult(
            success=result["valid"],
            data=result
        )
    
    async def handle_mesh(self, params: Dict[str, Any]) -> ToolResult:
        """メッシュ最適化"""
        gm3d_path = params.get("gm3d_path")
        
        result = await mesh_handler.process(gm3d_path)
        
        return ToolResult(
            success=result["success"],
            data=result
        )
    
    async def handle_diagnose(self, params: Dict[str, Any]) -> ToolResult:
        """CSG失敗診断"""
        content = params.get("gm3d_content")
        operations = params.get("operations", [])
        
        result = await diagnose_handler.process(content, operations)
        
        return ToolResult(
            success=True,
            data=result
        )

if __name__ == "__main__":
    server = AdvCADMCPServer()
    asyncio.run(server.start())
```

## 4. Web Application with MCP Client / MCP クライアント付きWebアプリ

### Architecture with MCP
```
┌─────────────────┐
│   Web Browser   │
└────────┬────────┘
         │
┌────────▼────────┐
│   Web Server    │
│  (FastAPI/Flask)│
└────────┬────────┘
         │
┌────────▼────────┐
│   MCP Client    │──────┐
└─────────────────┘      │
                         │ MCP Protocol
┌─────────────────┐      │
│ AI Assistant    │──────┤
│ (Claude/GPT-4)  │      │
└─────────────────┘      │
                         │
┌────────▼────────────────┐
│  AdvCAD MCP Server      │
├─────────────────────────┤
│ • generate_gm3d         │
│ • validate_gm3d         │
│ • optimize_mesh         │
│ • diagnose_csg_failure  │
│ • interpret_error       │
└─────────────────────────┘
         │
┌────────▼────────┐
│   AdvCAD Core   │
└─────────────────┘
```

### Web App as MCP Client
```python
from mcp.client import MCPClient
from fastapi import FastAPI, Request
from fastapi.responses import JSONResponse

app = FastAPI()
mcp_client = MCPClient()

@app.on_event("startup")
async def startup():
    # Connect to AdvCAD MCP server
    await mcp_client.connect("localhost", 8765)
    
@app.post("/api/generate")
async def generate_model(request: Request):
    data = await request.json()
    description = data.get("description")
    
    # Use MCP tool instead of direct implementation
    result = await mcp_client.call_tool(
        "generate_gm3d",
        {
            "description": description,
            "track_operations": True
        }
    )
    
    if result.success:
        # Continue with mesh optimization
        mesh_result = await mcp_client.call_tool(
            "optimize_mesh",
            {"gm3d_path": result.data["file_path"]}
        )
        
        if not mesh_result.success:
            # Diagnose failure
            diagnosis = await mcp_client.call_tool(
                "diagnose_csg_failure",
                {
                    "gm3d_content": result.data["gm3d_content"],
                    "operations": result.data["operations"]
                }
            )
            
            return JSONResponse({
                "success": False,
                "diagnosis": diagnosis.data
            })
    
    return JSONResponse(result.data)
```

## 5. MCP Context Management / MCPコンテキスト管理

### Stateful Conversations
```python
class ModelContext:
    """MCPで共有されるモデルコンテキスト"""
    def __init__(self):
        self.current_model = None
        self.operations = []
        self.mesh_parameters = {}
        self.error_history = []
    
    def to_mcp_context(self):
        return {
            "model": self.current_model,
            "operations": self.operations,
            "parameters": self.mesh_parameters,
            "errors": self.error_history
        }
```

### MCP Resources
```json
{
  "resources": [
    {
      "type": "model",
      "id": "current_model",
      "data": {
        "gm3d_content": "...",
        "operations": [...],
        "validation_status": "valid",
        "mesh_status": "optimized"
      }
    }
  ]
}
```

## 6. Multi-Agent Collaboration / マルチエージェント協調

### Agent Roles via MCP
```yaml
geometry_agent:
  role: "Generate initial geometry"
  tools: ["generate_gm3d"]
  
validation_agent:
  role: "Validate and verify topology"
  tools: ["validate_gm3d", "diagnose_csg_failure"]
  
optimization_agent:
  role: "Optimize mesh parameters"
  tools: ["optimize_mesh"]
  
interpreter_agent:
  role: "Explain errors in Japanese"
  tools: ["interpret_error"]
```

### Collaboration Flow
```python
async def collaborative_generation(description: str):
    # Geometry agent generates
    geometry = await geometry_agent.call("generate_gm3d", {
        "description": description
    })
    
    # Validation agent checks
    validation = await validation_agent.call("validate_gm3d", {
        "content": geometry["gm3d_content"]
    })
    
    if not validation["valid"]:
        # Interpreter agent explains
        interpretation = await interpreter_agent.call("interpret_error", {
            "error_log": validation["errors"]
        })
        return interpretation
    
    # Optimization agent handles mesh
    mesh = await optimization_agent.call("optimize_mesh", {
        "gm3d_path": geometry["file_path"]
    })
    
    return mesh
```

## 7. Deployment Configuration / デプロイメント構成

### Docker Compose with MCP
```yaml
version: '3.8'

services:
  advcad-mcp-server:
    build: ./mcp-server
    ports:
      - "8765:8765"
    volumes:
      - ./temp:/tmp
      - ./advcad:/usr/local/bin
    environment:
      - MCP_PORT=8765
      - ADVCAD_PATH=/usr/local/bin/advcad
  
  web-app:
    build: ./web-app
    ports:
      - "5000:5000"
    depends_on:
      - advcad-mcp-server
    environment:
      - MCP_SERVER_URL=advcad-mcp-server:8765
      - OPENAI_API_KEY=${OPENAI_API_KEY}
  
  nginx:
    image: nginx:alpine
    ports:
      - "80:80"
    volumes:
      - ./nginx.conf:/etc/nginx/nginx.conf
    depends_on:
      - web-app
```

## 8. MCP Tool Testing / MCPツールテスト

### Test Suite
```python
import pytest
from mcp.client import MCPClient

@pytest.fixture
async def mcp_client():
    client = MCPClient()
    await client.connect("localhost", 8765)
    yield client
    await client.disconnect()

async def test_generate_simple_cube(mcp_client):
    result = await mcp_client.call_tool(
        "generate_gm3d",
        {"description": "2x2x2の立方体を作成"}
    )
    assert result.success
    assert "cube" in result.data["gm3d_content"]

async def test_csg_diagnosis(mcp_client):
    # Generate complex model
    result = await mcp_client.call_tool(
        "generate_gm3d",
        {"description": "立方体に大きすぎる穴を開ける"}
    )
    
    if not result.success:
        # Test diagnosis
        diagnosis = await mcp_client.call_tool(
            "diagnose_csg_failure",
            {
                "gm3d_content": result.data["gm3d_content"],
                "operations": result.data["operations"]
            }
        )
        assert "failed_operation" in diagnosis.data
```

## 9. Benefits of MCP Integration / MCP統合の利点

1. **Standardization**: 標準プロトコルで他のツールと連携
2. **Reusability**: 複数のAIアシスタントから利用可能
3. **Composability**: 他のMCPツールと組み合わせ可能
4. **Maintainability**: ツール定義とロジックの分離
5. **Scalability**: マイクロサービスアーキテクチャ対応

## 10. Future MCP Extensions / 将来のMCP拡張

1. **CAD File System MCP Server**
   - Manage .gm3d file library
   - Version control integration

2. **Material Properties MCP Server**
   - Add material definitions
   - Physical simulation parameters

3. **Manufacturing MCP Server**
   - CNC toolpath generation
   - 3D printing preparation

4. **Collaboration MCP Server**
   - Multi-user editing
   - Change tracking