LDX #0
LDA hello, X
BRK
INX
LDA hello, X
BRK
INX
LDA hello, X
BRK
INX
LDA hello, X
BRK
INX
LDA hello, X
BRK
INX
LDA hello, X
BRK
INX
LDA hello, X
BRK
INX
LDA hello, X
BRK

.DATA
hello: .byte "hello\n", 0
