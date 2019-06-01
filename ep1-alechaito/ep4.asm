;*******************************************************************************
; UFSC- Universidade Federal de Santa Catarina
; Projeto: primeiro exemplo de programa??o assembler para PIC
; Autor: Roderval Marcelino
; Programa de testes das entradas e acionamento das sa?das
;
;*******************************************************************************

#include </opt/microchip/mplabx/v5.05/mpasmx/p16f877a.inc> 	; Arquivo de include do uC usado, no caso PIC16F877A



; Palavra de configura??o, desabilita e habilita algumas op??es internas


  __CONFIG  _CP_OFF & _CPD_OFF & _DEBUG_OFF & _LVP_OFF & _WRT_OFF & _BODEN_OFF & _PWRTE_OFF & _WDT_OFF & _XT_OSC


;***** defini??o de Vari?veis  *****************************
;cria constantes para o programa

  	cblock 0x20
	
		tempo0		
		tempo1			; Vari?veis usadas na rotina de delay.
		filtro
		temp			; Vari?vel tempor?ria.	
	endc 
	
	cblock 0x70
		work_temp
		status_temp		; Vari?veis usadas para salvar contexto no momento das interrup??es.
		fsr_temp
		pclath_temp	
	endc



;********* vari?veis de Entrada ************
#define	    entrada1	PORTB,2		; Est? ligada ao pino 1 do PortB (Pino 34 do uC)
#define	    entrada2	PORTB,3	
	
#define	    interr	PORTB,0
;********* vari?veis de Sa?da ***************
#define 	led	PORTD,3	            ; Sa?da que est? ligada ao led, pino 7 do port D

;******** Vetor de Reset do uC**************
org 0x00		                ; Vetor de reset do uC.
goto inicio		            ; Desvia para o inicio do programa.

;******** Vetor de Interrup??es do uC **************
org 0x04	                	; Vetor de interrup??es do uC.
salva_contexto:
	movwf	work_temp	        ; Salva o registrador de trabalho
	swapf	STATUS,W	        ; Salva o registrador de status
	movwf	status_temp
	movf	FSR,W
	movwf	fsr_temp	        ; Salva registrador FSR.
	movf	PCLATH,W
	movwf	pclath_temp	        ; Salva registrador PCLATH.
	clrf	PCLATH		        ; Seleciona p?gina de mem?ria 0.
	clrf	STATUS	        	; Seleciona banco de mem?ria 0.

	;Se foi RB0 eu trato a interrupcao
	btfss	INTCON,INTF	        ; Foi interrup??o externa pino RB0?
	goto	sai_int		        ; Nao - Sai da interrupcao.
	
	
; Tratamento da interrup??o de mudan?a de estado no PortB
int_rb0:
    	movlw	.250
	call	delay_ms
	
	bcf	led
	
	movf	PORTB,W
	
	btfss	interr
	goto	int_rb0
	
	clrf	temp		; Limpa vari?vel tempor?ria.

sai_int_rb0:
	bcf INTCON,INTF		; Limpa flag da interrup??o de mudan?a de estado no PortB .

;Recupera o contexto e sai da interrupcao
sai_int
	movf	pclath_temp,W
	movwf	PCLATH		; Recupera registrador PCLATH.
	movf	fsr_temp,W
	movwf	FSR		; Recupera registrador FSR.
	swapf	status_temp,W
	movwf	STATUS		; Recupera registrador STATUS.
	swapf	work_temp,F
	swapf	work_temp,W	; Recupera registrador Work.
 retfie


;*************** Rotinas e Sub-Rotinas *****************************************

; Rotina de delay (De 1ms at? 256 ms)


delay_ms:
	movwf	tempo1		; Carrega tempo1 (Unidade de ms).
	movlw	.250
	movwf	tempo0		; Carrega tempo0 (Para contar 1ms).
	nop					; Perde tempo.
	decfsz	tempo0,F	; Fim de tempo0 ?
	goto	$-2			; N?o - Volta duas instru??es.
						; Sim - Passou-se 1ms.
	decfsz	tempo1,F	; Fim de tempo1 ?
	goto	$-6			; N?o - Volta seis instru??es.
						; Sim.
 return   			; Retorna.
clean_bits_0:
	movf	PORTD,W
	incfsz	PORTC,1
	bcf	PORTD,0
	bcf	PORTD,1
	bcf	PORTD,2
	bcf	PORTD,3
	movlw	.250
	call	delay_ms
	return
	
clean_bits_1:
	movf	PORTC,W
	bcf	PORTC,0
	bcf	PORTC,1
	bcf	PORTC,2
	bcf	PORTC,3
	movlw	.250
	call	delay_ms
	return
	
test_bit:
	;Display menos significativo
	movf	PORTD,W	
	btfsc	PORTD,0
	call	clean_bits_0
	;Display mais significativo
	movf	PORTC,W	
	btfsc	PORTC,0
	call	clean_bits_1
	
	return
verify_bits:
	;Display menos significativo
	movf	PORTD,W	
	btfsc	PORTD,3
	call	test_bit
	;Display mais significativo
	movf	PORTC,W	
	btfsc	PORTC,3
	call	test_bit
	return
    
trata_entrada1:
	decfsz	filtro,F
	goto	trata_entrada1
ligado:	    
	movf	PORTD,W	
	
	movlw	.250
	call	delay_ms
	
	call	verify_bits
	
	incfsz	PORTD,1
	
    	return



;****************** Inicio do programa *****************************************

inicio:
	clrf	PORTA		; Inicializa os Port's. Coloca todas pinos em 0.
	clrf	PORTB
	clrf	PORTC
	clrf	PORTD
	clrf	PORTE

	banksel	TRISA		; Seleciona banco de mem?ria 1
	movlw	0x1f
	movwf   TRISA		; Configura PortA (A0,A1,A2,A3,A4 = Entradas, A5 = Sa?da).
	movlw	0xff
	movwf	TRISB		; Configura PortB como entrada.
	movlw	0x00
	movwf	TRISC		; Configura PortC (C4,C7 = Sa?das, C0,C1,C2,C3,C5,C6 = Entradas).
	movlw	0x00
	movwf	TRISD		; Configura PortD como sa?da.
	movlw	0x07
	movwf	TRISE		; Configura PortE como entrada e desliga Posta Paralela.

	movlw	0x00
	movwf	OPTION_REG	; Configura Op??es:
				; Pull-Up habilitados.
				; Interrup??o na borda de subida do sinal no pino B0.
				; Timer0 incrementado pelo oscilador interno.
				; Prescaler WDT 1:1.
				; Prescaler Timer0 1:2.

	movlw	0x90		; 00000011
	movwf	INTCON		; Habilitando interrupcoes externas

	movlw	0x00
	movwf	PIE1		; Desabilita interrup??es perif?ricas.

	movlw	0x00
	movwf	PIE2		; Desabilita interrup??es perif?ricas.

	movlw	0x07
	movwf	ADCON0		; Desliga conversor A/D, PortA e PortE com I/O digital.

	movlw	0x07
	movwf	CMCON		; Desliga comparadores internos.

	movlw	0x00
	movwf	CVRCON		; Desliga m?dulo de ref?rencia interna de tens?o.

	banksel PORTA		; Seleciona banco de mem?ria 0.



;*********************** Loop principal ****************************************
loop:
	movf	PORTB,W			; L? o PortB.

	btfss	entrada1	    ; A entrada "1" foi acionada?
	call	trata_entrada1	; Sim - Trata a entrada "1".
	

	movlw 	.250
	movwf	filtro		    ; Carrega filtro das entradas. Tempo para soltar o bot?o e n?o chamar a subrotina novamente

	goto 	loop		    ; Vai para loop. Deixa lendo as entradas 

    end			            ; Fim do Programa.
