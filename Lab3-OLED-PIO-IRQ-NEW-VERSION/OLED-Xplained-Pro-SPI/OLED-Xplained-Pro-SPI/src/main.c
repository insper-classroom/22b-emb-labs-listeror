/************************************************************************/
/* COMPUTAÇÃO EMBARCADA                                                 */
/* Developed by: Lister Ogusuku Ribeiro                                 */
/* Engenharia de Computação - Insper                                    */
/* 06/01/2023                                                           */
/************************************************************************/


/************************************************************************/
/* includes                                                             */
/************************************************************************/

#include "asf.h"
#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"

/************************************************************************/
/* defines                                                              */
/************************************************************************/

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

volatile char but1_flag;
volatile int frequencia = 500;
volatile char soltou_botao = 0;

/************************************************************************/
/* prototype                                                            */
/************************************************************************/
void io_init(void);
void pisca_led(int n);
void but_callback(void);
void but1_callback(void);
void but2_callback(void);
void but3_callback(void);
void atualiza_frequencia(void);
void limpa_barra_de_progresso(void);

/************************************************************************/
/* handler / callbacks                                                  */
/************************************************************************/

/*
 * Exemplo de callback para o botao, sempre que acontecer
 * ira piscar o led por 5 vezes
 *
 * !! Isso é um exemplo ruim, nao deve ser feito na pratica, !!
 * !! pois nao se deve usar delays dentro de interrupcoes    !!
 */
void but_callback(void){
	
	
}

void but1_callback(void){
	but1_flag = 1;
	if (pio_get(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK)) {
		// PINO == 1 --> Borda de subida
		soltou_botao = 0;
		} else {
		// PINO == 0 --> Borda de descida
		soltou_botao = 1;
		
	}
	
}

void but2_callback(void){
	
}

void but3_callback(void){
	
}


/************************************************************************/
/* funções                                                              */
/************************************************************************/

// pisca led N vez no periodo T
void pisca_led(int n){
	
	int j = 0;
	for (int i=0;i<n;i++){
		
		gfx_mono_draw_rect(i*25, 5, 19, 10, GFX_PIXEL_SET);
		delay_ms(10);
		j+=2;

		pio_clear(LED_PIO, LED_PIO_IDX_MASK);
		delay_ms(frequencia);
		pio_set(LED_PIO, LED_PIO_IDX_MASK);
		delay_ms(frequencia);
  }
  
  limpa_barra_de_progresso();
}


void aumenta_frequencia(void){
	frequencia -= 100;
	
}

void diminui_frequencia(void){
	frequencia += 100;
}

void atualiza_frequencia(void){
	if (frequencia < 100){
		frequencia = 500;
	}
	
	//int cnt = 5;
	char str[128];

	sprintf(str, "freq = %d Hz", frequencia);
	gfx_mono_draw_string(str, 0, 18, &sysfont);
	
	
	
}

void limpa_barra_de_progresso(void){
	
	for(int i=120;i>=0;i-=2){
		
		gfx_mono_draw_rect(i, 5, 2, 10, GFX_PIXEL_CLR);
		delay_ms(10);
		
	}
}



// Inicializa botao SW0 do kit com interrupcao
// Inicializa botao SW0 do kit com interrupcao
void io_init(void)
{

	// Configura led
	// Ativa PIOs
	// LED E BOTÃO DA PLACA
	pmc_enable_periph_clk(LED_PIO_ID);
	pmc_enable_periph_clk(BUT_PIO_ID); // Inicializa clock do periférico PIO responsavel pelo botao

	// LED 1 E BOTÃO 1 DO OLED
	pmc_enable_periph_clk(LED1_PIO_ID);
	pmc_enable_periph_clk(BUT1_PIO_ID);

	// LED 2 E BOTÃO 2 DO OLED
	pmc_enable_periph_clk(LED2_PIO_ID);
	pmc_enable_periph_clk(BUT2_PIO_ID);

	// LED 3 E BOTÃO 3 DO OLED
	pmc_enable_periph_clk(LED3_PIO_ID);
	pmc_enable_periph_clk(BUT3_PIO_ID);
	

	// Configura PIO para lidar com o pino do botão como entrada com pull-up
	pio_configure(BUT_PIO, PIO_INPUT, BUT_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT_PIO, BUT_PIO_IDX_MASK, 60);
	
	pio_configure(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT1_PIO, BUT1_PIO_IDX_MASK, 60);
	
	pio_configure(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT2_PIO, BUT2_PIO_IDX_MASK, 60);
	
	pio_configure(BUT3_PIO, PIO_INPUT, BUT3_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT3_PIO, BUT3_PIO_IDX_MASK, 60);
	
	
	// Configura LEDS como saídas:
	pio_configure(LED_PIO, PIO_OUTPUT_0, LED_PIO_IDX_MASK, PIO_DEFAULT);
	pio_configure(LED1_PIO, PIO_OUTPUT_0, LED1_PIO_IDX_MASK, PIO_DEFAULT);
	pio_configure(LED2_PIO, PIO_OUTPUT_0, LED2_PIO_IDX_MASK, PIO_DEFAULT);
	pio_configure(LED3_PIO, PIO_OUTPUT_0, LED3_PIO_IDX_MASK, PIO_DEFAULT);
	

	// Configura interrupção no pino referente ao botao e associa
	// função de callback caso uma interrupção for gerada
	// a função de callback é a: but_callback()
	pio_handler_set(BUT_PIO,
					BUT_PIO_ID,
					BUT_PIO_IDX_MASK,
					PIO_IT_EDGE,
					but_callback);
	
	pio_handler_set(BUT1_PIO,
					BUT1_PIO_ID,
					BUT1_PIO_IDX_MASK,
					PIO_IT_EDGE,
					but1_callback);
	
	pio_handler_set(BUT2_PIO,
					BUT2_PIO_ID,
					BUT2_PIO_IDX_MASK,
					PIO_IT_RISE_EDGE,
					but2_callback);
	
	pio_handler_set(BUT3_PIO,
					BUT3_PIO_ID,
					BUT3_PIO_IDX_MASK,
					PIO_IT_RISE_EDGE,
					but3_callback);

	// Ativa interrupção e limpa primeira IRQ gerada na ativacao
	pio_enable_interrupt(BUT_PIO, BUT_PIO_IDX_MASK);
	pio_get_interrupt_status(BUT_PIO);
	
	pio_enable_interrupt(BUT1_PIO, BUT1_PIO_IDX_MASK);
	pio_get_interrupt_status(BUT1_PIO);
	
	pio_enable_interrupt(BUT2_PIO, BUT2_PIO_IDX_MASK);
	pio_get_interrupt_status(BUT2_PIO);
	
	pio_enable_interrupt(BUT3_PIO, BUT3_PIO_IDX_MASK);
	pio_get_interrupt_status(BUT3_PIO);
	
	// Configura NVIC para receber interrupcoes do PIO do botao
	// com prioridade 4 (quanto mais próximo de 0 maior)
	NVIC_EnableIRQ(BUT_PIO_ID);
	NVIC_SetPriority(BUT_PIO_ID, 4); // Prioridade 4
	
	NVIC_EnableIRQ(BUT1_PIO_ID);
	NVIC_SetPriority(BUT1_PIO_ID, 4); // Prioridade 4
	
	NVIC_EnableIRQ(BUT2_PIO_ID);
	NVIC_SetPriority(BUT2_PIO_ID, 3); // Prioridade 3
	
	NVIC_EnableIRQ(BUT3_PIO_ID);
	NVIC_SetPriority(BUT3_PIO_ID, 5); // Prioridade 5
}


int main (void)
{
	board_init();
	sysclk_init();
	delay_init();
	// Desativa watchdog
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	// configura botao com interrupcao
	io_init();

    // Init OLED
	gfx_mono_ssd1306_init();
	
	atualiza_frequencia();
  
  
	//gfx_mono_draw_filled_circle(20, 16, 16, GFX_PIXEL_SET, GFX_WHOLE);
    //gfx_mono_draw_string("mundo", 50,16, &sysfont);
  
  

    /* Insert application code here, after the board has been initialized. */
	while(1) {
		
			if (but1_flag){
				
				
				aumenta_frequencia();
				atualiza_frequencia();
				pisca_led(5);
				delay_ms(500);
				while (soltou_botao != 0){
					diminui_frequencia();
					atualiza_frequencia();
					pisca_led(5);
					delay_ms(500);
				}
				
				but1_flag = 0;
			}
			pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);

			
	}
}
