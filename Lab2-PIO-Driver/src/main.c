/**
 * 5 semestre - Eng. da Computação - Insper
 * Rafael Corsi - rafael.corsi@insper.edu.br
 *
 * Projeto 0 para a placa SAME70-XPLD
 *
 * Objetivo :
 *  - Introduzir ASF e HAL
 *  - Configuracao de clock
 *  - Configuracao pino In/Out
 *
 * Material :
 *  - Kit: ATMEL SAME70-XPLD - ARM CORTEX M7
 */

/************************************************************************/
/* includes                                                             */
/************************************************************************/

#include "asf.h"

/************************************************************************/
/* defines                                                              */
/************************************************************************/

/*  Default pin configuration (no attribute). */
#define _PIO_DEFAULT             (0u << 0)
/*  The internal pin pull-up is active. */
#define _PIO_PULLUP              (1u << 0)
/*  The internal glitch filter is active. */
#define _PIO_DEGLITCH            (1u << 1)
/*  The internal debouncing filter is active. */
#define _PIO_DEBOUNCE            (1u << 3)

// LED E BOTÃO DA PLACA


#define LED_PIO           PIOC                 // periferico que controla o LED
#define LED_PIO_ID        ID_PIOC                  // ID do periférico PIOC (controla LED)
#define LED_PIO_IDX       8                    // ID do LED no PIO
#define LED_PIO_IDX_MASK  (1 << LED_PIO_IDX)   // Mascara para CONTROLARMOS o LED

// Configuracoes do botao
#define BUT_PIO				PIOA
#define BUT_PIO_ID			ID_PIOA
#define BUT_PIO_IDX			11
#define BUT_PIO_IDX_MASK (1u << BUT_PIO_IDX) // esse já está pronto.


// LEDS E BOTÕES DO OLED

//LED E BOTÃO 1:

#define LED1_PIO           PIOA                 // periferico que controla o LED
#define LED1_PIO_ID        ID_PIOA                  // ID do periférico PIOC (controla LED)
#define LED1_PIO_IDX       0                    // ID do LED no PIO (PIO A PINO 0)
#define LED1_PIO_IDX_MASK  (1 << LED1_PIO_IDX)   // Mascara para CONTROLARMOS o LED

// Configuracoes do botao
#define BUT1_PIO			PIOD
#define BUT1_PIO_ID			ID_PIOD
#define BUT1_PIO_IDX		28
#define BUT1_PIO_IDX_MASK (1u << BUT1_PIO_IDX)


//LED E BOTÃO 2:

#define LED2_PIO           PIOC                 // periferico que controla o LED
#define LED2_PIO_ID        ID_PIOC                  // ID do periférico PIOC (controla LED)
#define LED2_PIO_IDX       30                    // ID do LED no PIO (PIO A PINO 0)
#define LED2_PIO_IDX_MASK  (1 << LED2_PIO_IDX)   // Mascara para CONTROLARMOS o LED

// Configuracoes do botao
#define BUT2_PIO			PIOC
#define BUT2_PIO_ID			ID_PIOC
#define BUT2_PIO_IDX		31
#define BUT2_PIO_IDX_MASK (1u << BUT2_PIO_IDX)


//LED E BOTÃO 3:

#define LED3_PIO           PIOB                 // periferico que controla o LED
#define LED3_PIO_ID        ID_PIOB                  // ID do periférico PIOC (controla LED)
#define LED3_PIO_IDX       2                    // ID do LED no PIO (PIO A PINO 0)
#define LED3_PIO_IDX_MASK  (1 << LED3_PIO_IDX)   // Mascara para CONTROLARMOS o LED

// Configuracoes do botao
#define BUT3_PIO			PIOA
#define BUT3_PIO_ID			ID_PIOA
#define BUT3_PIO_IDX		19
#define BUT3_PIO_IDX_MASK (1u << BUT3_PIO_IDX)

/************************************************************************/
/* constants                                                            */
/************************************************************************/

/************************************************************************/
/* variaveis globais                                                    */
/************************************************************************/

/************************************************************************/
/* prototypes                                                           */
/************************************************************************/

void init(void);

/************************************************************************/
/* interrupcoes                                                         */
/************************************************************************/

/************************************************************************/
/* funcoes                                                              */
/************************************************************************/
void _pio_set(Pio *p_pio, const uint32_t ul_mask)
{
	p_pio->PIO_SODR = ul_mask;
}

/**
 * \brief Set a low output level on all the PIOs defined in ul_mask.
 * This has no immediate effects on PIOs that are not output, but the PIO
 * controller will save the value if they are changed to outputs.
 *
 * \param p_pio Pointer to a PIO instance.
 * \param ul_mask Bitmask of one or more pin(s) to configure.
 */
void _pio_clear(Pio *p_pio, const uint32_t ul_mask)
{
	p_pio->PIO_CODR = ul_mask;
}

/**
 * \brief Configure PIO internal pull-up.
 *
 * \param p_pio Pointer to a PIO instance.
 * \param ul_mask Bitmask of one or more pin(s) to configure.
 * \param ul_pull_up_enable Indicates if the pin(s) internal pull-up shall be
 * configured.
 */
void _pio_pull_up(Pio *p_pio, const uint32_t ul_mask,
        const uint32_t ul_pull_up_enable){
			
			/* Enable the pull-up(s) if necessary */
			if (ul_pull_up_enable) {
				p_pio->PIO_PUER = ul_mask;
				} else {
				p_pio->PIO_PUDR = ul_mask;
			}

 }
 
 /**
 * \brief Configure one or more pin(s) or a PIO controller as inputs.
 * Optionally, the corresponding internal pull-up(s) and glitch filter(s) can
 * be enabled.
 *
 * \param p_pio Pointer to a PIO instance.
 * \param ul_mask Bitmask indicating which pin(s) to configure as input(s).
 * \param ul_attribute PIO attribute(s).
 
 */

void _pio_set_input(Pio *p_pio, const uint32_t ul_mask,
const uint32_t ul_attribute)
{
	if (ul_attribute && _PIO_PULLUP){
		_pio_pull_up(p_pio, ul_mask, 1);
	}
	if (ul_attribute && _PIO_DEGLITCH){
		p_pio->PIO_IFSCDR = ul_mask;
		}else{
		if (ul_attribute && _PIO_DEBOUNCE){
			p_pio->PIO_IFSCER = ul_mask;
		}
	}
	
	p_pio->PIO_ODR = ul_mask; // Disable register (output)
	p_pio->PIO_PER = ul_mask; // Periferical Enable register
}

/**
 * \brief Configure one or more pin(s) of a PIO controller as outputs, with
 * the given default value. Optionally, the multi-drive feature can be enabled
 * on the pin(s).
 *
 * \param p_pio Pointer to a PIO instance.
 * \param ul_mask Bitmask indicating which pin(s) to configure.
 * \param ul_default_level Default level on the pin(s).
 * \param ul_multidrive_enable Indicates if the pin(s) shall be configured as
 * open-drain.
 * \param ul_pull_up_enable Indicates if the pin shall have its pull-up
 * activated.
 */
void _pio_set_output(Pio *p_pio, const uint32_t ul_mask,
        const uint32_t ul_default_level,
        const uint32_t ul_multidrive_enable,
        const uint32_t ul_pull_up_enable)
{
	//Configurar o PIO para controlar o pino
	p_pio->PIO_PER = ul_mask;
	
	//Configurar o pino em modo saída
	p_pio->PIO_OER = ul_mask;
	
	//Definir a saída inicial do pino (1 ou 0)
	if (ul_default_level){
		_pio_set(p_pio, ul_mask);
	}else{
		_pio_clear(p_pio, ul_mask);
	}
	
	//Ativar ou não o multidrive :
	if (ul_multidrive_enable){
		p_pio->PIO_MDER = ul_mask;
	}else{
		p_pio->PIO_MDDR = ul_mask;
	}
	
	//Ativar ou não o pull-up :
	if (ul_pull_up_enable){
		_pio_pull_up(p_pio, ul_mask,1);
	}
}

uint32_t _pio_get(Pio *p_pio, const pio_type_t ul_type,
const uint32_t ul_mask)
{
	uint32_t statusRegister;
	if (ul_type == PIO_OUTPUT_0) {
		statusRegister = p_pio->PIO_ODSR;
		} else {
		statusRegister = p_pio->PIO_PDSR;
	}
	if(ul_mask & statusRegister){
		return 1;
		}else{
		return 0;
	}
}

void _delay_ms(int n){
	for(int i = 0; i<(500*n*1000); i++){
		asm("NOP");
	}
}

// Função de inicialização do uC
void init(void){
	// Inicializa clock
	sysclk_init();

	// Desativa watchdog
	WDT->WDT_MR = WDT_MR_WDDIS;

	// Ativa PIOs
	// LED E BOTÃO DA PLACA
	pmc_enable_periph_clk(LED_PIO_ID);
	pmc_enable_periph_clk(BUT_PIO_ID);

	// LED 1 E BOTÃO 1 DO OLED
	pmc_enable_periph_clk(LED1_PIO_ID);
	pmc_enable_periph_clk(BUT1_PIO_ID);

	// LED 2 E BOTÃO 2 DO OLED
	pmc_enable_periph_clk(LED2_PIO_ID);
	pmc_enable_periph_clk(BUT2_PIO_ID);

	// LED 3 E BOTÃO 3 DO OLED
	pmc_enable_periph_clk(LED3_PIO_ID);
	pmc_enable_periph_clk(BUT3_PIO_ID);

	// Configura Pinos
	
	_pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 0, 0, 0);
	_pio_set_input(BUT_PIO, BUT_PIO_IDX_MASK, PIO_DEFAULT);
	_pio_pull_up(BUT_PIO, BUT_PIO_IDX_MASK, 1);
	
	//pio_configure(LED_PIO, PIO_OUTPUT_0, LED_PIO_IDX_MASK, PIO_DEFAULT);
	//pio_configure(BUT_PIO, PIO_INPUT, BUT_PIO_IDX_MASK, PIO_PULLUP);

	pio_configure(LED1_PIO, PIO_OUTPUT_0, LED1_PIO_IDX_MASK, PIO_DEFAULT);
	pio_configure(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK, PIO_PULLUP);

	//pio_configure(LED2_PIO, PIO_OUTPUT_0, LED2_PIO_IDX_MASK, PIO_DEFAULT);
	//pio_configure(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK, PIO_PULLUP);

	_pio_set_output(LED2_PIO, LED2_PIO_IDX_MASK, 0, 0, 0);
	_pio_set_input(BUT2_PIO, BUT2_PIO_IDX_MASK, PIO_DEFAULT);
	_pio_pull_up(BUT2_PIO, BUT2_PIO_IDX_MASK, 1);
	
	pio_configure(LED3_PIO, PIO_OUTPUT_0, LED3_PIO_IDX_MASK, PIO_DEFAULT);
	pio_configure(BUT3_PIO, PIO_INPUT, BUT3_PIO_IDX_MASK, PIO_PULLUP);

}

/************************************************************************/
/* Main                                                                 */
/************************************************************************/

// Funcao principal chamada na inicalizacao do uC.
int main(void)
{
  init();

  // super loop
  // aplicacoes embarcadas não devem sair do while(1).
  while(1) {
	  // Verifica valor do pino que o botão está conectado
	  if(!_pio_get(BUT_PIO, PIO_INPUT, BUT_PIO_IDX_MASK)) {
		  // Pisca LED
		  for (int i=0; i<10; i++) {
			  _pio_clear(LED_PIO, LED_PIO_IDX_MASK);  // Limpa o pino LED_PIO_PIN
			  _delay_ms(100);                         // delay
			  _pio_set(LED_PIO, LED_PIO_IDX_MASK);    // Ativa o pino LED_PIO_PIN
			  _delay_ms(100);                         // delay
		  }
		  } else  {
		  // Ativa o pino LED_IDX (par apagar)
		  _pio_set(LED_PIO, LED_PIO_IDX_MASK);
	  }
	if(!_pio_get(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK)) {
		// Pisca LED
		for (int i=0; i<10; i++) {
			_pio_clear(LED1_PIO, LED1_PIO_IDX_MASK);  // Limpa o pino LED_PIO_PIN
			_delay_ms(100);                         // delay
			_pio_set(LED1_PIO, LED1_PIO_IDX_MASK);    // Ativa o pino LED_PIO_PIN
			_delay_ms(100);                         // delay
		}
		} else  {
		// Ativa o pino LED_IDX (par apagar)
		_pio_set(LED1_PIO, LED1_PIO_IDX_MASK);
	}

	if(!_pio_get(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK)) {
		// Pisca LED
		for (int i=0; i<10; i++) {
			_pio_clear(LED2_PIO, LED2_PIO_IDX_MASK);  // Limpa o pino LED_PIO_PIN
			_delay_ms(100);                         // delay
			_pio_set(LED2_PIO, LED2_PIO_IDX_MASK);    // Ativa o pino LED_PIO_PIN
			_delay_ms(100);                         // delay
		}
		} else  {
		// Ativa o pino LED_IDX (par apagar)
		_pio_set(LED2_PIO, LED2_PIO_IDX_MASK);
	}
	if(!_pio_get(BUT3_PIO, PIO_INPUT, BUT3_PIO_IDX_MASK)) {
		// Pisca LED
		for (int i=0; i<10; i++) {
			_pio_clear(LED3_PIO, LED3_PIO_IDX_MASK);  // Limpa o pino LED_PIO_PIN
			_delay_ms(100);                         // delay
			_pio_set(LED3_PIO, LED3_PIO_IDX_MASK);    // Ativa o pino LED_PIO_PIN
			_delay_ms(100);                         // delay
		}
		} else  {
		// Ativa o pino LED_IDX (par apagar)
		_pio_set(LED3_PIO, LED3_PIO_IDX_MASK);
	}
  }
  return 0;
}
