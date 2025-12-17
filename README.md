```text
project/
├─ core/                   # 순수 C++ 핵심 로직
│   ├─ VehicleData.cpp/h    # 속도, RPM, 연료 등
│   ├─ CANInterface.cpp/h   # CAN 통신 처리
│   └─ Utils.cpp/h          # 공용 유틸
│
├─ apps/
│   ├─ Dashboard/           # 계기판 앱
│   │   ├─ main.cpp
│   │   ├─ DashboardUI.qml  # QML UI
│   │   └─ DashboardController.cpp/h
│   │
│   └─ Infotainment/        # 인포테인먼트 앱
│       ├─ main.cpp
│       ├─ InfotainmentUI.qml
│       └─ InfotainmentController.cpp/h
│
├─ ipc/                     # 앱 간 통신 (API 또는 네트워크)
│   ├─ IPCServer.cpp/h
│   └─ IPCClient.cpp/h
│
├─ CMakeLists.txt           # 전체 프로젝트 빌드 정의
└─ README.md
```