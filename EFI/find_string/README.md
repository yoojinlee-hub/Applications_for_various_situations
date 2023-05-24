-. 아래와 같은 기능의 EFI Shell application을 제작한다.
1) 임의의 파일명과 string을 parameter로 받아, 받은 파일에서 받은 string을 검색해서 결과를 return 한다.
2) 결과는 found인 경우 0으로 return 하며 ResultPass.log 파일을 만들고, not found인 경우 1로 return 한다.
-. 아래 메일에서와 같이 테스트 하는 script를 2가지로 만든다.
      1) ResultPass.log 파일의 존재 여부에 따라 Pass / Fail을 display 한다.
      2) 위의 EFI Shell application을 return 결과에 따라 Pass / Fail을 display 한다.

