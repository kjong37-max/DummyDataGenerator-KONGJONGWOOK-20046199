# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 프로젝트 개요

이 프로젝트는 **Dummy 데이터 생성 Tool의 PoC(Proof of Concept) 개발** 프로젝트입니다.

- **목적**: 테스트용 Dummy 데이터를 자동으로 생성하는 도구 개발
- **핵심 기능**:
  1. 사용자 정의 스키마 기반으로 Dummy 데이터 생성
  2. 생성된 데이터를 연결된 DB(데이터베이스)에 직접 삽입하는 기능

## 기술 스택

- **언어**: C++ (C++20 표준)
- **플랫폼**: Windows Console Application
- **빌드 시스템**: Visual Studio 2022 MSBuild (v145 toolset)
- **지원 아키텍처**: Win32 / x64

## 빌드 명령

### Visual Studio IDE
솔루션 파일(`DummyDataGenerator.slnx`)을 Visual Studio 2022에서 열고 빌드.

### MSBuild CLI
```powershell
# Debug 빌드 (x64)
msbuild DummyDataGenerator_project.vcxproj /p:Configuration=Debug /p:Platform=x64

# Release 빌드 (x64)
msbuild DummyDataGenerator_project.vcxproj /p:Configuration=Release /p:Platform=x64
```

빌드 결과물은 각각 `x64\Debug\` 또는 `x64\Release\` 디렉터리에 생성됩니다.

## 아키텍처 방향

PoC 단계에서 구현할 주요 컴포넌트:

| 컴포넌트 | 역할 |
|---|---|
| **Schema Parser** | 데이터 생성 규칙(컬럼 타입, 범위, 패턴 등)을 정의하는 설정 파싱 |
| **Data Generator** | 스키마 규칙에 따라 랜덤/규칙 기반 Dummy 데이터 생성 |
| **DB Connector** | 생성된 데이터를 대상 DB에 삽입 (ODBC 또는 특정 DB 드라이버 활용 예정) |
| **CLI Interface** | 생성 건수, 대상 테이블, DB 연결 정보 등을 커맨드라인으로 입력받는 인터페이스 |

소스 파일은 Visual Studio 필터 기준으로 `소스 파일(*.cpp)` / `헤더 파일(*.h, *.hpp)` 로 구분합니다.

## 개발 시 주의사항

- DB 연결 문자열(Connection String), 계정 정보 등 민감한 값은 소스 코드에 하드코딩하지 않고 설정 파일 또는 환경 변수로 분리합니다.
- 이 프로젝트는 PoC이므로, 지원할 DB 종류와 데이터 타입의 범위를 초기에 명확히 제한하여 범위를 관리합니다.
