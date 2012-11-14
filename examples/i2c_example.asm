
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;                                                                     
;                     Software License Agreement                      
;                                                                     
; The software supplied herewith by Microchip Technology Incorporated 
; (the "Company") for its PICmicro® Microcontroller is intended and   
; supplied to you, the Company’s customer, for use solely and         
; exclusively on Microchip PICmicro Microcontroller products.         
;                                                                     
; The software is owned by the Company and/or its supplier, and is     
; protected under applicable copyright laws. All rights are reserved.  
; Any use in violation of the foregoing restrictions may subject the  
; user to criminal sanctions under applicable laws, as well as to     
; civil liability for the breach of the terms and conditions of this  
; license.                                                             
;                                                                      
; THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,   
; WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED   
; TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A         
; PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,   
; IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR          
; CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.                    
;                                                                     
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;
;   Filename:           AN989.asm
;   Date:               May 9, 2005
;   File Version:       1.0
;   Assembled using:    MPLAB IDE 7.11.00.0
;
;   Author:             Chris Parris
;   Company:            Microchip Technology, Inc.
;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;
;   Files required:     p18f452.inc
;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;
;   Purpose:
;
;   This application note is intended to serve as a reference for
;   communicating with Microchip’s I2C serial EEPROM devices with the
;   use of the PICmicro MSSP module on PIC18 devices.
;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;
;   Program Description:
;
;   This program illustrates the following I2C operations:
;    - Byte Write
;    - Byte Read
;    - Page Write
;    - Sequential Read
;   For the byte operations, 1 data byte is read from/written to
;   the 24LC256 device. For the page write and sequential read, 16
;   data bytes are transferred. This number is set through the
;   NUMBYTES constant, and so can be changed to read/write a different
;   number of bytes. Note that if you attempt to cross a page boundary
;   during a write operation, the address will wrap back to the
;   beginning of the page, overwriting any previously written bytes.
;
;   All operations start at address 0x5A00. All timings are set to meet
;   the 100 kHz spec, and assume a 10 MHz crystal oscillator is used.
;   If a different crystal frequency is desired, the value of SSPADD
;   (currently 0x18) must be updated in Init. Also, note that no action
;   is taken if the device does not respond with an ACK bit when one is
;   expected.
;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  LIST P=18F452
; Port C pin descriptions
; SCL                   bit = 3
; SDA                   bit = 4
;
; 10MHz crystal is being used, thus each instruction cycle = 400 ns
;
;*******************RAM register definitions**********************
    cblock  0x000
        datai                   ; Data input byte buffer
        datao                   ; Data output byte buffer
        bytecount               ; Counter for byte loops
        pollcnt                 ; Counter for polling loops
        loops                   ; Delay loop counter
        loops2                  ; Delay loop counter
    endc
;*******************Macro definitions*****************************
WRITE_ADDR  equ     0x3a		; Control byte for write operations
READ_ADDR   equ     0x3b		; Control byte for read operations
NUMBYTES    equ     .1         ; # of bytes for page/seq. operations
;*******************Include file**********************************
    include "p18f452.inc"       ; Include file for the PIC18F452
    errorlevel  -302            ; Suppress message 302 from list file
;*****************************************************************
    org     0x000               ; Set the reset vector
    goto    start               ; Go to the beginning of main
;*******************Begin Main Program****************************
start
    rcall   Init                ; Initialize device
    ;rcall   ByteWrite           ; Byte Write operation 
    rcall   ByteRead            ; Byte Read operation

    bra     $                   ; Loop here forever

;*******************Initialization subroutine*********************
;           This routine initializes the MSSP module
;           for I2C Master mode, with a 100 kHz clock.
;*****************************************************************
Init
    movlw   b'11111111'
    movwf   TRISC               ; Set PORTC to all inputs
    clrf    SSPSTAT             ; Disable SMBus inputs
    bsf     SSPSTAT,SMP         ; Disable slew rate control
    movlw   0x18                ; Load 0x18 into WREG
    movwf   SSPADD              ; Setup 100 kHz I2C clock
    movlw   b'00101000'
    movwf   SSPCON1             ; Enable SSP, select I2C Master mode
    clrf    SSPCON2             ; Clear control bits
    bcf     PIR1,SSPIF          ; Clear SSP interrupt flag
    bcf     PIR2,BCLIF          ; Clear Bit Collision flag
    return

;*******************Start bit subroutine**************************
;           This routine generates a Start condition
;           (high-to-low transition of SDA while SCL
;           is still high.
;*****************************************************************
BSTART
    bcf     PIR1,SSPIF          ; Clear SSP interrupt flag
    bsf     SSPCON2,SEN         ; Generate Start condition
bstart_wait
    btfss   PIR1,SSPIF          ; Check if operation completed
    bra     bstart_wait         ; If not, keep checking

    return

;*******************Restart bit subroutine**************************
;           This routine generates a Repeated Start
;           condition (high-to-low transition of SDA
;           while SCL is still high.
;*****************************************************************
BRESTART
    bcf     PIR1,SSPIF          ; Clear SSP interrupt flag
    bsf     SSPCON2,RSEN        ; Generate Restart condition
brestart_wait
    btfss   PIR1,SSPIF          ; Check if operation completed
    bra     brestart_wait       ; If not, keep checking

    return

;*******************Stop bit subroutine***************************
;           This routine generates a Stop condition
;           (low-to-high transition of SDA while SCL
;           is still high.
;*****************************************************************
BSTOP
    bcf     PIR1,SSPIF          ; Clear SSP interrupt flag
    bsf     SSPCON2,PEN         ; Generate Stop condition
bstop_wait
    btfss   PIR1,SSPIF          ; Check if operation completed
    bra     bstop_wait          ; If not, keep checking

    return

;*******************Data transmit subroutine**********************
;           This routine transmits the byte of data
;           stored in 'datao' to the serial EEPROM
;           device. Instructions are also in place
;           to check for an ACK bit, if desired.
;           Just replace the 'bra' instruction,
;           or create an 'ackfailed' label, to provide
;           the functionality.
;*****************************************************************
TX_BYTE
    bcf     PIR1,SSPIF          ; Clear SSP interrupt flag
    movf    datao,W             ; Copy datao to WREG
    movwf   SSPBUF              ; Write byte out to device
tx_wait
    btfss   PIR1,SSPIF          ; Check if operation completed
    bra     tx_wait             ; If not, keep checking
;   btfsc   SSPCON2,ACKSTAT     ; Check if ACK bit was received
;   bra     ackfailed           ; This executes if no ACK received    

    return

;*******************Data receive subroutine***********************
;           This routine reads in one byte of data from
;           the serial EEPROM device, and stores it in
;           'datai'.  It then responds with either an
;           ACK or a NO ACK bit, depending on the value
;           of 'ACKDT' in 'SSPCON2'.
;*****************************************************************
RX_BYTE
    bcf     PIR1,SSPIF          ; Clear SSP interrupt flag
    bsf     SSPCON2,RCEN        ; Initiate reception of byte
rx_wait
    btfss   PIR1,SSPIF          ; Check if operation completed
    bra     rx_wait             ; If not, keep checking
    movf    SSPBUF,W            ; Copy byte to WREG
    movwf   datai               ; Copy WREG to datai
    bcf     PIR1,SSPIF          ; Clear SSP interrupt flag
    bsf     SSPCON2,ACKEN       ; Generate ACK/NO ACK bit    
rx_wait2
    btfss   PIR1,SSPIF          ; Check if operation completed
    bra     rx_wait2            ; If not, keep checking

    return

;*******************Byte write test subroutine********************
;           This routine tests the byte write feature
;           of the serial EEPROM device.  It will write
;           1 byte of data to the device at address 0x5A00.
;*****************************************************************
ByteWrite
    rcall   BSTART              ; Generate Start condition

                                ; Send control byte
    movlw   WRITE_ADDR          ; Load control byte for write
    movwf   datao               ; Copy to datao for output
    rcall   TX_BYTE             ; Send control byte to device

                                ; Send word address high byte
    movlw   0x5A                ; Load 0x5A for word address
    movwf   datao               ; Copy to datao for output
    rcall   TX_BYTE

                                ; Send word address low byte
    movlw   0x00                ; Load 0x00 for word address
    movwf   datao               ; Copy to datao for output
    rcall   TX_BYTE             ; Send word address to device

                                ; Send data byte
    movlw   0xAA                ; Load 0xAA for data byte
    movwf   datao               ; Copy to datao for output
    rcall   TX_BYTE             ; Send data byte to device

    rcall   BSTOP               ; Generate Stop condition

    rcall   Poll                ; Poll for write completion

    return

;*******************Page write test subroutine********************
;           This routine demonstrates the page write
;           feature of the serial EEPROM device.  It will
;           write NUMBYTES number of bytes of data to the
;           device starting at address 0x5A00.  Since this
;           is a page write, all bytes will be written
;           to the device at the same time.
;           Note that if a page boundary is crossed during
;           the operation, the address will wrap back to
;           the beginning of the page, overwriting any
;           previously stored data.
;*****************************************************************
PageWrite
    movlw   NUMBYTES
    movwf   bytecount           ; Initialize counter to NUMBYTES

    rcall   BSTART              ; Generate start bit
                                ; Now we send the control byte
    movlw   WRITE_ADDR
    movwf   datao               ; Copy control byte to buffer
    rcall   TX_BYTE             ; Output control byte to device

                                ; Send word address high byte
    movlw   0x5A                ; Use 0x5A for address to send
    movwf   datao               ; Copy address to buffer
    rcall   TX_BYTE

                                ; Send word address low byte
    movlw   0x00                ; Use 0x00 for address to send
    movwf   datao               ; Copy address to buffer
    rcall   TX_BYTE             ; Output address to device

txbyte                          ; This loop transfers all of
                                ; the data bytes to the device
    movf    bytecount,W         ; Use bytecount as data value
    movwf   datao               ; Copy data to buffer
    rcall   TX_BYTE             ; Output data to device

    decfsz  bytecount,F         ; Check if finished looping
    bra     txbyte              ; Continue looping

    rcall   BSTOP               ; Generate stop bit
    rcall   Poll                ; Poll for write completion
    return

;*******************Byte read test subroutine*********************
;           This routine tests the byte read feature
;           of the serial EEPROM device.  It will read
;           1 byte of data at address 0x5A00 from the device.
;*****************************************************************
ByteRead
    rcall   BSTART              ; Generate Start condition

                                ; Send control byte
    movlw   WRITE_ADDR          ; Load control byte for write
    movwf   datao               ; Copy to datao for output
    rcall   TX_BYTE             ; Send control byte to device

                                ; Send word address high byte
    ;movlw   0x5A                ; Load 0x5A for word address
    ;movwf   datao               ; Copy to datao for output
    ;rcall   TX_BYTE             ; Send high byte to device

                                ; Send word address low byte
    movlw   0x00                ; Load 0x00 for word address
    movwf   datao               ; Copy to datao for output
    rcall   TX_BYTE             ; Send word address to device

    rcall   BRESTART            ; Generate Restart condition

                                ; Send control byte
    movlw   READ_ADDR           ; Load control byte for read
    movwf   datao               ; Copy to datao for output
    rcall   TX_BYTE             ; Send control byte to device

                                ; Read data byte
    bsf     SSPCON2,ACKDT       ; Select to send NO ACK bit
    rcall   RX_BYTE             ; Read data byte from device

    rcall   BSTOP               ; Generate Stop condition

    return

;*******************Sequential read test subroutine***************
;           This routine tests the sequential read feature
;           of the serial EEPROM device.  It will read
;           NUMBYTES number of bytes of data to the device
;           starting at address 0x5A00, one byte after another.
;*****************************************************************
SequentialRead
    movlw   NUMBYTES
    movwf   bytecount           ; Initialize counter to NUMBYTES

    rcall   BSTART              ; Generate start bit
                                ; Now send the control byte
                                ; for a write, to set address
    movlw   WRITE_ADDR
    movwf   datao               ; Copy control byte to buffer
    rcall   TX_BYTE             ; Output control byte to device

                                ; Send word address high byte
    movlw   0x5A                ; Load 0x5A for word address
    movwf   datao               ; Copy to datao for output
    rcall   TX_BYTE             ; Send high byte to device

                                ; Send word address low byte
    movlw   0x00                ; Load 0x5A for word address
    movwf   datao               ; Copy to datao for output
    rcall   TX_BYTE             ; Send word address to device

    rcall   BRESTART            ; Generate another start bit
                                ;   to switch to read mode
    movlw   READ_ADDR
    movwf   datao               ; Copy control byte to buffer
    rcall   TX_BYTE             ; Output control byte to device

rxbyte
                                ; Finally, read the data byte
    decfsz  bytecount,F         ; Check if finished looping
    bra     continue            ; If not finished, keep going
    bsf     SSPCON2,ACKDT       ; Otherwise, select to send NO ACK bit
    rcall   RX_BYTE             ;   and input final byte from device

    rcall   BSTOP               ; Generate stop bit
    return

continue
    bcf     SSPCON2,ACKDT       ; Select to send ACK bit
    rcall   RX_BYTE             ; Input data from device
    bra     rxbyte              ; Continue looping

;*******************Acknowledge Polling subroutine****************
;           This subroutine polls the EEPROM device
;           for an ACK bit, which indicates that the
;           internal write cycle has completed. Code
;           is in place for a timeout routine, just
;           uncomment the 'bra TimedOut' line, and
;           provide a 'TimedOut' label.
;*****************************************************************
Poll
    movlw   .40
    movwf   pollcnt             ; Set max polling times to 40
polling
    rcall   BRESTART            ; Generate start bit
    movlw   WRITE_ADDR          ; Now send the control byte
    movwf   datao               ; Copy control byte to buffer
    rcall   TX_BYTE             ; Output control byte to device
    btfss   SSPCON2,ACKSTAT     ; Was the ACK bit low?
    bra     exitpoll            ; If yes, stop polling
                                ; If no, check if polled 40 times
    decfsz  pollcnt,F           ; Is poll counter down to zero?
    bra     polling             ; If no, poll again
;   bra     TimedOut            ; If yes, part didn't respond
                                ; in time, so take action
exitpoll
    rcall   BSTOP               ; Generate stop bit
    return

    END
