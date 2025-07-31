# 🎮 TOGU : Planet Survivors

![PlayDemoGIF](https://github.com/user-attachments/assets/9257c9b1-f15a-492e-9788-a3118e2ce21c)

**언리얼 5.4 기반 로그라이크 Top-down 슈팅 게임 | 개인 프로젝트 (2024.05 ~ 2024.07)**
---

## 🔗 데모 및 실시간 시연 영상
- ▶️ [**Play Demo (시스템 설명 포함)**](https://youtu.be/1-GPB7u94ic)
- 🎮 [**직접 플레이용 빌드 다운로드**](https://drive.google.com/drive/folders/1s_h8lspvXMmB7Nm8zqnCxm9qfz-Yd-3D?usp=sharing)
- 📄 [**코드 설명 PDF**](https://github.com/user-attachments/files/21538989/_._2.pdf)
---


## 🧠 시스템 아키텍처 개요 (UML 기반)

| 시스템 이름 | 구조 이미지 |
|-------------|-------------|
| 리워드 시스템 구조 예시 | <img width="1661" height="603" alt="RewardSystemUML" src="https://github.com/user-attachments/assets/31e6ad5a-8174-4bc7-b887-685da3a2093e" /> |
| 오브젝트 풀링 구조 예시 | <img width="1514" height="419" alt="ObjectPoolUML" src="https://github.com/user-attachments/assets/8d5cfb42-c472-4d3d-a8bb-a8d04b06e8e9" /> |

*UML은 실제 코드와 대응되며, 각 시스템은 다음과 같은 원칙에 기반해 설계되었습니다.*

---

## ⚙️ 주요 시스템 설명

### 🟡 1. Object Pooling System
- `UObjectPoolManager`, `FPoolData`, `Acquire() / Release()` 패턴 기반
- 실시간 디버그 UI로 풀 상태 시각화
- GC 및 Spawn 비용 최소화를 위한 구조

---

### 🟢 2. Reward System
- `URewardManager`, `URewardSelector`, `URewardSelectionWidget` 간의 MVC 구조
- `IRewardData`, `IRewardApplicator` 언리얼 인터페이스 기반 확장성 확보
- UMG 연동으로 레벨업 보상 UI 구현

---

### 🔵 3. Data-Driven Balancing
- 웨이브 구성: `UWaveConfigDataAsset`
- 적 스탯 구성: `UEnemyDataAsset`
- 플레이어 스탯: `UPlayerDataAsset`
- CSV 및 DataTable 기반으로 디자이너가 직접 밸런싱 가능

- 📄 [**디자이너용 가이드: 신규 무기/아이템 추가법**](https://ethereal-judo-1f1.notion.site/223486e2cdb980c5a807f920ebad70a6)
- 📄 [**디자이너용 가이드: 신규 몬스터 추가법**](https://ethereal-judo-1f1.notion.site/223486e2cdb98001869cef28bb9bfbb5)

---

### 🔴 4. MetaSound 연동
- `ClimateFixCue` Beat 콜백에 따라 적 스폰 트리거
- 사운드 연동으로 긴장감 있는 전투 연출

---

## 🧩 기타 게임 시스템

### ▪️ Planet Torque 식 DayOfWeek 시스템
- 마우스 회전값 누적으로 요일 변화
- 요일별 무기/아이템 기능 트리거

### ▪️ Super Hot like 플레이어 입력 기반 AI
- 더 큰/더 빠른 마우스 입력값 -> 더 빠른 적 AI
- 마우스 입력을 멈출 경우 -> 멈추고 제자리에서 가만히 있음

---

## 🔧 개발 기술 스택

`C++`, `Unreal Engine 5.4`, `UMG`, `UInterface`, `BlueprintCallable`, `UObjectPool`,  
`UDataTable`, `Delegate`, `Enhanced Input`, `Component Architecture`, `Modular OOP`, `MetaSound`

---

## ✍️ 느낀 점

- 언리얼 메모리 모델(GC, CDO, UPROPERTY)을 겪으며 메모리 구조 이해
- MVC 아키텍처, 인터페이스 기반 확장, 리팩토링 능력 향상
- 디자이너 친화적인 구조를 고려한 Data-Driven 시스템과 문서화 가이드를 직접 제작
