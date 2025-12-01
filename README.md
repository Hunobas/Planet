# 🎮 TOGU : Planet Survivors

![PlayDemoGIF](https://github.com/user-attachments/assets/9257c9b1-f15a-492e-9788-a3118e2ce21c)

**언리얼 5.4 기반 로그라이크 Top-down 슈팅 게임 | 개인 프로젝트 (2025.05 ~ 2025.07)**

---

## 🔗 프로젝트 링크

- ▶️ [**Play Demo (시스템 설명 포함)**](https://youtu.be/1-GPB7u94ic)
- 🎮 [**직접 플레이용 빌드 다운로드**](https://drive.google.com/drive/folders/1s_h8lspvXMmB7Nm8zqnCxm9qfz-Yd-3D?usp=sharing)
- 📄 [**TOGU 코드설명.pdf**](https://github.com/user-attachments/files/21698200/TOGU.pdf)
- 📘 [**전체 포트폴리오**](https://github.com/Hunobas/Portfolio)

---

## 🎯 프로젝트 목표

대규모 오브젝트 생성/소멸로 인한 성능 병목을 해결하고, 확장 가능한 시스템 아키텍처를 설계하여 디자이너 친화적인 밸런싱 환경을 구축합니다.

---

## 🚀 핵심 성과

### 📊 Object Pooling을 통한 성능 최적화

**GC 스레드 호출 빈도 80% 이상 감소**

<img width="1256" height="644" alt="image" src="https://github.com/user-attachments/assets/25ce068f-e07b-4c4d-b2a3-726d83b97d55" />

> *위 이미지: 오브젝트 풀링 적용 전후 비교*
> - **적용 전**: 빈번한 GC 호출로 인한 프레임 드랍 발생
> - **적용 후**: GC 스레드 호출 빈도가 대폭 감소하여 안정적인 프레임 유지

#### 기술적 구현
- `TCircularQueue` 기반 lock-free 구조로 멀티스레드 안전성 확보
- `FScopeLock`을 활용한 크리티컬 섹션 보호
- `AcquireOrNull()` / `Release()` 패턴으로 명확한 생명주기 관리
- 실시간 디버그 UI로 풀 상태 시각화
```cpp
template<typename T>
T* AcquireOrNull(const TSubclassOf<T>& _actorClass, const FTransform& _spawnTransform)
{
    FScopeLock lock(&mPoolCriticalSection);
    FPoolData* poolData = getOrCreatePoolData(_actorClass);
    
    AActor* actor = nullptr;
    if (!poolData->Available.Dequeue(actor))
    {
        actor = createNewActor(_actorClass, _spawnTransform);
    }
    
    if (IsValid(actor))
    {
        actor->SetActorTransform(_spawnTransform);
        setActorActiveState(actor, true);
    }
    
    return Cast<T>(actor);
}
```

**성능 개선 수치**
- 적 최대 동시 생존 수: 100+ 마리
- 프레임 안정성: 60 FPS 유지
- GC 호출 빈도: 기존 대비 80% 이상 감소

---

## 🧠 시스템 아키텍처 개요 (UML 기반)

| 시스템 이름 | 구조 이미지 |
|-------------|-------------|
| 리워드 시스템 구조 예시 | <img width="1661" height="603" alt="RewardSystemUML" src="https://github.com/user-attachments/assets/31e6ad5a-8174-4bc7-b887-685da3a2093e" /> |
| 오브젝트 풀링 구조 예시 | <img width="1514" height="419" alt="ObjectPoolUML" src="https://github.com/user-attachments/assets/8d5cfb42-c472-4d3d-a8bb-a8d04b06e8e9" /> |

---

## ⚙️ 주요 시스템

### 🟡 1. Object Pooling System

**설계 목표**: 대규모 오브젝트 생성/소멸 시 발생하는 GC 비용 최소화

**핵심 구현**
- `UObjectPoolManager`: 전역 풀 관리자
- `FPoolData`: 클래스별 풀 데이터 구조체
- `TCircularQueue<AActor*>`: 순환 큐 기반 풀 컨테이너
- Thread-safe 구조: `FCriticalSection` 활용

**주요 기능**
- 동적 풀 크기 조절 (초기: 256, 필요 시 자동 확장)
- BeginPlay 시점 프리로드로 런타임 부하 감소
- 실시간 디버그 UI로 풀 상태 모니터링

---

### 🟢 2. Reward System

**설계 목표**: 확장 가능한 보상 시스템 아키텍처 구축

**MVC 패턴 기반 구조**
- **Model**: `IRewardData` 인터페이스 (데이터 추상화)
- **View**: `URewardSelectionWidget` (UMG 기반 UI)
- **Controller**: `URewardManager` (비즈니스 로직)

**확장성 확보**
- `IRewardApplicator` 인터페이스로 적용 로직 분리
- DataTable 기반으로 신규 보상 추가 시 코드 수정 불필요
- 디자이너가 직접 밸런싱 가능한 구조

---

### 🔵 3. Data-Driven Balancing

**설계 목표**: 프로그래머 개입 없이 디자이너가 게임 밸런싱 가능

**DataAsset 구조**
- `UWaveConfigDataAsset`: 웨이브 구성 (적 종류, 수, 타이밍)
- `UEnemyDataAsset`: 적 스탯 (HP, 공격력, 이동속도)
- `UPlayerDataAsset`: 플레이어 스탯 및 성장 곡선

**디자이너 문서**
- 📄 [**신규 무기/아이템 추가 가이드**](https://ethereal-judo-1f1.notion.site/223486e2cdb980c5a807f920ebad70a6)
- 📄 [**신규 몬스터 추가 가이드**](https://ethereal-judo-1f1.notion.site/223486e2cdb98001869cef28bb9bfbb5)

**장점**
- CSV/DataTable 기반으로 Excel에서 직접 편집 가능
- 블루프린트 노출로 언리얼 에디터 내 즉시 수정
- 버전 관리 용이 (텍스트 기반 에셋)

---

## 🧩 독창적인 게임 시스템

### ▪️ Planet Torque 식 DayOfWeek 시스템

**컨셉**: 마우스 회전으로 행성을 돌려 요일 변경

**구현**
- 마우스 회전값 누적으로 요일 전환
- 요일별 무기/아이템 기능 활성화
- 전략적 타이밍 선택이 핵심

---

### ▪️ Super Hot like 시간 제어 시스템

**컨셉**: 플레이어 입력에 따라 시간 흐름 제어

**구현**
- 마우스 입력 강도 → 적 AI 속도 비례
- 입력 중지 → 시간 정지 (전술적 사고 시간 제공)
- 전투의 긴장감과 전략성 동시 확보

---

### 🔴 MetaSound 연동 전투 시스템

**컨셉**: 배경 음악의 BPM에 맞춰 원거리 적의 공격을 트리거

**구현 방식**
- `ClimateFixCue` Beat 콜백에 따라 적 스폰 트리거
- BPM 기반 웨이브 타이밍 조절
- 음악 진행도에 따른 난이도 조절

---

## 🔧 기술 스택

### 언어 & 엔진
`C++`, `Unreal Engine 5.4`, `Blueprint`

### 핵심 기술
`UObject Pool`, `UInterface`, `DataTable`, `Enhanced Input`, `MetaSound`

### 아키텍처 패턴
`MVC`, `Component Architecture`, `Data-Driven Design`, `Observer Pattern (Delegate)`

### 메모리 관리
`UPROPERTY`, `TSharedPtr`, `FScopeLock`, `GC Optimization`

---

## 📈 프로젝트 성과 요약

| 항목 | 내용 |
|------|------|
| **성능 최적화** | GC 스레드 호출 빈도 80% 이상 감소 |
| **확장성** | 인터페이스 기반 모듈화로 신규 시스템 추가 용이 |
| **협업 친화성** | 디자이너가 코드 수정 없이 밸런싱 가능 |
| **아키텍처** | SOLID 원칙 준수, UML 기반 설계 |

---

## ✍️ 배운 점

**언리얼 메모리 모델 이해**
- GC, CDO, UPROPERTY를 통한 언리얼 메모리 관리 체계 학습
- 스마트 포인터(`TSharedPtr`, `TWeakPtr`)의 적절한 활용법 습득

**아키텍처 설계 능력 향상**
- MVC 패턴을 게임 시스템에 적용하여 관심사 분리
- 인터페이스 기반 설계로 확장 가능한 구조 구축
- 리팩토링을 통한 코드 품질 개선 경험

**협업 관점 이해**
- 디자이너 친화적인 Data-Driven 구조 설계
- 명확한 문서화의 중요성 체감
- 비프로그래머도 이해할 수 있는 시스템 설계 경험

---

*이 프로젝트는 개인 포트폴리오 목적으로 제작되었으며, 상업적 이용을 목적으로 하지 않습니다.*
