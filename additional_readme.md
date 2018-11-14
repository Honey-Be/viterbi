# 소스코드 저장소  
    - [Honey-Be/viterbi Github repo](https://github.com/Honey-Be/viterbi)
  
# 컴파일 명령(Clang-cl 기준)  
    - clang-cl /Oi /O3 /std:c++14 /guard:cf /I "<저장소 폴더>\include" main.cpp fileutil.cpp observation.cpp phone.cpp transition.cpp viterbi.cpp voca.cpp  