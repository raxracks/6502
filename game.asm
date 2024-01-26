.segment "CODE"

    .proc main
main:
    ; Initialize PPU
    LDA #%10000000   ; Set the bit 7 to $2000 to use $2000-$2FFF for nametables
    STA $2000
    LDA #%00011110   ; Enable rendering, enable sprites, and make sure background is visible
    STA $2001

    ; Load sprite data into OAM
    LDX #0           ; Initialize X register to zero
    LDA SpriteData,X ; Load the first byte of sprite data
    STA $0200,X      ; Store it in the Object Attribute Memory (OAM)
    INX              ; Increment X to load the next byte
    LDA SpriteData,X ; Load the second byte of sprite data (attributes)
    STA $0200,X      ; Store it in OAM
    INX              ; Increment X to load the next byte (X position)
    LDA SpriteData,X ; Load the third byte of sprite data (X position)
    STA $0200,X      ; Store it in OAM

    ; Infinite loop
    forever:
        JMP forever

    .endproc

.segment "DATA"

SpriteData:
    .byte $00  ; Tile index for the sprite
    .byte $20  ; Attributes (palette, flipping, etc.)
    .byte $80  ; X position of the sprite
    ; Additional sprite data can be added here

.segment "BSS"

    ; Define uninitialized memory (if needed)


