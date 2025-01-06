;
; PLATFORM = 0 : Homebrew FPGA
; PLATFORM = 1 : Homebrew Hardware
; PLATFORM = 2 : Atari XL/XE
; PLATFORM = 3 : my65816 FPGA
; PLATFORM = 4 : Atari XL/XE FAT12
;
;
; FAT-16 defines
;
;       DOS 2           MyDOS
;-------------------------------------
;3	OPEN        	OPEN
;5	GET RECORD	GET RECORD
;7	GET CHARACTERS	GET CHARACTERS
;9	PUT RECORD	PUT RECORD
;11	PUT CHARACTERS	PUT CHARACTERS
;12	CLOSE	        CLOSE
;13	STATUS REQUEST	STATUS REQUEST
;32	RENAME          RENAME
;33	DELETE	        DELETE
;34		        CREATE DIRECTORY
;35	LOCK FILE	LOCK FILE
;36	UNLOCK FILE	UNLOCK FILE
;37	POINT[1]	POINT[1]
;38	NOTE[1]	        NOTE[1]
;39		        LOAD BINARY FILE&RUN
;40		        LOAD BINARY FILE&RUN
;41		        CHANGE DIRECTORY	
;42		        CREATE DIRECTORY

DRVCHAR		= 'D'

	.IF PLATFORM <= 1
FATPTRS		= $0006
FATPTRD		= $0008
	.ELSE
FATPTRS		= $0000
FATPTRD		= $0002
SECTOR		= FMSZPG
SHIFT		= FMSZPG+4
	.ENDIF

;--------------------------------------------------------------
; Memory Setup
;--------------------------------------------------------------
	.IF PLATFORM <= 1
VBASE		= $400
	.ENDIF
	
;	.IF PLATFORM = 3
;VBASE		= $400
;	.ENDIF
	
	.IF PLATFORM = 2 || PLATFORM = 4
VBASE		= $700
FATCODE		= VBASE+$600
DUPCODE		= $1b00
	.ENDIF
	
	.IF PLATFORM = 2
JSTARTUP	= FATCODE
JPRINT		= FATCODE+3
JPUTHEX		= FATCODE+6
JPRINTSTR	= FATCODE+9
JINITFAT	= FATCODE+12
	.ENDIF
	
	.IF PLATFORM = 4
JSTARTUP	= STARTUP
JPRINT		= PRINT
JPUTHEX		= PUTHEX
JPRINTSTR	= PRINTSTR
JINITFAT	= INITFAT
	.ENDIF

V		= VBASE
BUFFER		= VBASE+$200
FATBUF		= VBASE+$400
	
CRENAME		= 32
CDELETE		= 33
CCREDIR		= 34
CFSTAT		= 40
CCHADIR		= 41
CCREDIR1	= 42
	
DEL		= $E5
EOF		= 136
	
SECCLUS		= V+0		;1
RESSEC		= V+1		;2
NUMFATS		= V+3		;1
NUMDIR		= V+4		;2
NUMSECS		= V+6		;2
MEDIA		= V+8		;1
SECSFAT		= V+9		;2
FCBCODE		= V+11 		;1
		
dummy		= V+12		;4
FATBASE		= V+16		;4
DIRBASE		= V+20		;4
DATBASE		= V+24		;4
LASTSEC		= V+28		;4
LASTFAT		= V+32		;4
BYTCLUS		= V+36 		;4
		
BIN		= V+40 		;4
BCD		= V+44 		;5
DECIMAL		= V+49 		;10
		
NEWCLUS		= V+59		;2
DDIRTY		= V+61 		;1
FDIRTY		= V+62 		;1
BUFADRL		= V+63 		;1
BUFADRH		= V+64 		;1
		
DIRCLUS		= V+65		;2
DIRRENA		= V+67 		;11
		
NODISPL		= SHIFT		;V+78 		;1
WILD		= V+79 		;1
DIRLINE		= V+80 		;13
DIRLENG		= V+93 		;10
DIREOF		= V+103 	;1
SAVEY		= V+104		;1
VALIDS		= V+105		;1-byte number of specials chars to check for
HILF		= V+106		;1-byte aux usage	
ASTART		= V+107		;2-byte file load start vector
AEND		= V+109		;2-byte file load end vector
CNT		= V+111		;1-byte adress enter counter DUP
FHEADER		= V+112		;2-byte save file header ($FF)
FSTART		= V+114		;2-byte file load start vector save
FEND		= V+116		;2-byte file load end vector save
FLENGTH		= V+118		;2-byte file length (for DUP save function)
JMPIND		= V+120		;1-byte holding $4C for JMP
JMPPTR		= V+121		;2-byte jmp address
		
F		= V+122
		
;-------------------------------------------------------------------------------
; current File-Control-Block
; is copied from FCB0-3
;-------------------------------------------------------------------------------
FIOCBNR		= F		;1-byte FCB is assigned to this IOCB
FFCBOFF		= F+1		;1-byte Offset of this FCB from FCB0
FSTRTCL		= F+2		;2-byte Start cluster of file
FILELEN		= F+4		;4-byte length of file
FWBURST		= F+8		;1-byte write burst 0=OFF, other=ON
FBURST		= F+9		;1-byte flag indicating burst read ($FF = slow)
FRPOS		= F+10		;4-byte bytes already read
FVAR32		= F+14		;4-byte 32-bit general var
FCURCLUS	= F+18		;4-byte current cluster
FCURSEC		= F+22		;1-byte current sector
FCURPTR		= F+23 		;2-byte current pointer into sector buffer
FSECTOR		= F+25 		;4-byte last sector read
		
;-------------------------------------------------------------------------------
; Directory variables
;-------------------------------------------------------------------------------
DIRTEXT		= F+29		;11-byte directory text 8 + 3
DIRPTR		= F+40		;2-byte pointer into directory sector
DIRSEC		= F+42		;4-byte directory sector
DIRFILE		= F+46		;1-byte dir or file to search for
FCBEND		= F+46		;FCB end-marker
;-------------------------------------------------------------------------------
; current FCB ends here
;-------------------------------------------------------------------------------

;-------------------------------------------------------------------------------
; space for 4 File-Control-Blocks
;-------------------------------------------------------------------------------
FCB0		= V+256
FCB1		= FCB0+64
FCB2		= FCB1+64
FCB3		= FCB2+64
	
DNAME		= 0
DEXT		= 8
DATTRIB		= 11
DCLUS		= $1A
DLEN		= $1C
PART1		= $1C6
	.IF PLATFORM <> 4
FATEND		= $FFFF
	.ELSE
FATEND		= $0FFF
	.ENDIF