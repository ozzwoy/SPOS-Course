<h1>FIXED BUGS</h1>

- Variable `virtPageNum` in `Kernel.java` means the number of virtual pages 
(this follows from the numerous comments across the code), hence added some 
fixes:

  - `line 9` changed `virtPageNum` initial value to 64 as it's mentioned in 
  User Guide, corrected the comment above
  
  - `lines 49, 71, 188` fixed a formula: 
    - was: `address_limit = (block * virtPageNum + 1) - 1`
    - now: `address_limit = block * virtPageNum - 1`
  
  - `line 65` fixed a formula: 
    - was: `virtPageNum = Common.s2i(st.nextToken()) - 1`
    - now: `virtPageNum = Common.s2i(st.nextToken())`
    
  - `line 66` fixed constraint:
    - was: `if ( virtPageNum < 2 || virtPageNum > 63 )`
    - now: `if ( virtPageNum < 2 || virtPageNum > 64 )`
    
  - `lines 77, 195` fixed loop's stop-condition:
    - was: `for (i = 0; i <= virtPageNum; i++)`
    - now: `for (i = 0; i < virtPageNum; i++)`
    
  - `line 105` fixed condition:
    - was: `id < 0 || id > virtPageNum`
    - now: `id < 0 || id >= virtPageNum`
    
  - `line 346` fixed a formula:
    - was: `high = block * virtPageNum`
    - now: `high = block * virtPageNum - 1`
  
- Among other fixes in `Kernel.java`:
  
  - `lines 288-330` fixed possible bug when giving physical 
  addresses. In order to avoid duplicating of PA, added boolean array
  which indicates currently used PA
  
  - `line 426` replaced `==` strings comparison with `equals`
  
  - `line 478` added `page.R = 1` on WRITE operation
    
- Renamed `Virtual2Physical.java` to `Utils.java` because no 
virtual-to-physical address transformations are done there.
Also made some fixes:

  - `line 9` fixed loop's stop-condition:
    - was: `for (i = 0; i <= numpages; i++)`
    - now: `for (i = 0; i < numpages; i++)`
    
- Swapped function parameters in `ControlPanel.java` in line 961:
    - was: `public void addPhysicalPage( int pageNum, int physicalPage )`
    - now: `public void addPhysicalPage( int physicalPage, int pageNum )`