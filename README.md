# DirectX Rendering Pipeline Practice

<br>

> SGI 아카데미 DirectX Rendering Pipeline Practice 사진 출력하기(**VS 2017 기준으로 제작하였음**)

* 버텍스, 인덱스 버퍼, 인덱스 레이아웃 만들어 IA State 제작
* .hlsl 파일을 이용해 버텍스 셰이더를 제작하여 VS State 제작
* RS State에 뷰 포트를 제공하여 그 외 정점 커팅
* .hlsl 파일을 이용해 픽셀 셰이더 제작, .png 파일을 이용해 셰이더 리소스 뷰 제작, 샘플러 스테이트 제작하여 PS State 제작
* 블렌드 스테이트 제작, 렌더 타겟 뷰 생성하여 OM State 제작 
* 최종적으로 DrawCall 호출하여 화면에 사진을 띄움
* SwapBuffer 방식을 이용한 깜빡임 방지

</br>
