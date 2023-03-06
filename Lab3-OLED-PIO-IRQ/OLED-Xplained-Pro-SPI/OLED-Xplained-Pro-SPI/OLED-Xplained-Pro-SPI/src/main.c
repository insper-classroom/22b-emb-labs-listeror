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

// LED
#define LED_PIO      PIOA
#define LED_PIO_ID   ID_PIOA
#define LED_IDX      0
#define LED_IDX_MASK (1 << LED_IDX)

// Configuracoes do Botão 1
#define BUT_PIO      PIOD
#define BUT_PIO_ID   ID_PIOD
#define BUT_IDX  28
#define BUT_IDX_MASK (1 << BUT_IDX)

//Configuracoes do botao 2
#define BUT2_PIO          PIOC
#define BUT2_PIO_ID       ID_PIOC
#define BUT2_PIO_IDX      31
#define BUT2_PIO_IDX_MASK (1 << BUT2_PIO_IDX)

// Configuracoes do botao 3
#define BUT3_PIO          PIOA
#define BUT3_PIO_ID       ID_PIOA
#define BUT3_PIO_IDX      19
#define BUT3_PIO_IDX_MASK (1 << BUT3_PIO_IDX)

/************************************************************************/
/* constants                                                            */
/************************************************************************/

/************************************************************************/
/* variaveis globais                                                    */
/************************************************************************/
/* flag */
volatile char but_flag; //
volatile char but_flag2; //
volatile char but_flag3; //
int n = 30;
int t = 300;

/************************************************************************/
/* prototype                                                            */
/************************************************************************/
void io_init(void);
void blink_led(void);
void but_callback(void);
void but_callback2(void);
void but_callback3(void);

/************************************************************************/
/* handler / callbacks                                                  */
/************************************************************************/
void but_callback(void)
{
	if (pio_get(BUT_PIO, PIO_INPUT, BUT_IDX_MASK)) {
		// PINO == 1 --> Borda de subida
		but_flag = 0; //soltou
		}else{
		// PINO == 0 --> Borda de descida
		but_flag = 1;  //apertou
	}
}

void but_callback2(void)
{
	but_flag2 = 1;  //apertou
}

void but_callback3(void)
{
	but_flag3 = 1;  //apertou
}

/************************************************************************/
/* funções                                                              */
/************************************************************************/
void atualiza_display(){
	//gfx_mono_draw_string("                                          ", 50, 16, &sysfont);
	char str[128]; //
	float f = (float) 500.0/t;
	sprintf(str, "%0.4f", f); //
	gfx_mono_draw_string("Hz:", 0, 16, &sysfont);
	gfx_mono_draw_string(str, 50, 16, &sysfont);
}
void limpa_barra(){
	
	for(int i=120;i>=0;i-=1){
		gfx_mono_draw_rect(i, 5, 2, 10, GFX_PIXEL_CLR);
		//delay_ms(10);
	}
	
}

// pisca led N vez no periodo T
void blink_led(){
	limpa_barra();
	for (int i=0;i<n;i++){
		atualiza_display();
		if(but_flag){
			limpa_barra();
			break;
		}
		
		if(but_flag2){
			but_flag2 = 0;
			limpa_barra();
			break;
		}
		
		if(but_flag3){
			//but_flag3 = 0;
			limpa_barra();
			break;
		}
		
		pio_clear(LED_PIO, LED_IDX_MASK);
		gfx_mono_draw_rect(i*4, 5, 2, 10, GFX_PIXEL_SET);
		delay_ms(t);
		pio_set(LED_PIO, LED_IDX_MASK);
		delay_ms(t);
	}
	limpa_barra();
	//pmc_sleep(SAM_PM_SMODE_SLEEP_WFI); //Código 'trava' aqui até ser 'acordado'
}

// Inicializa botao SW0 do kit com interrupcao
void io_init(void)
{

	// Configura led
	pmc_enable_periph_clk(LED_PIO_ID);
	pio_configure(LED_PIO, PIO_OUTPUT_0, LED_IDX_MASK, PIO_DEFAULT);

	// Inicializa clock do periférico PIO responsavel pelo botao
	pmc_enable_periph_clk(BUT_PIO_ID);
	pmc_enable_periph_clk(BUT2_PIO_ID);
	pmc_enable_periph_clk(BUT3_PIO_ID);

	// Configura PIO para lidar com o pino do botão como entrada
	// com pull-up
	
	pio_configure(BUT_PIO, PIO_INPUT, BUT_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT_PIO, BUT_IDX_MASK, 60);
	
	pio_configure(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT2_PIO, BUT2_PIO_IDX_MASK, 60);
	
	
	pio_configure(BUT3_PIO, PIO_INPUT, BUT3_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT3_PIO, BUT3_PIO_IDX_MASK, 60);

	// Configura interrupção no pino referente ao botao e associa
	// função de callback caso uma interrupção for gerada
	// a função de callback é a: but_callback()
	pio_handler_set(BUT_PIO,
	BUT_PIO_ID,
	BUT_IDX_MASK,
	PIO_IT_EDGE,
	but_callback);
	
	pio_handler_set(BUT2_PIO,
	BUT2_PIO_ID,
	BUT2_PIO_IDX_MASK,
	PIO_IT_RISE_EDGE,
	but_callback2);
	
	pio_handler_set(BUT3_PIO,
	BUT3_PIO_ID,
	BUT3_PIO_IDX_MASK,
	PIO_IT_RISE_EDGE,
	but_callback3);

	// Ativa interrupção e limpa primeira IRQ gerada na ativacao
	pio_enable_interrupt(BUT_PIO, BUT_IDX_MASK);
	pio_get_interrupt_status(BUT_PIO);
	
	pio_enable_interrupt(BUT2_PIO, BUT2_PIO_IDX_MASK);
	pio_get_interrupt_status(BUT2_PIO);
	
	pio_enable_interrupt(BUT3_PIO, BUT3_PIO_IDX_MASK);
	pio_get_interrupt_status(BUT3_PIO);
	
	// Configura NVIC para receber interrupcoes do PIO do botao
	// com prioridade 4 (quanto mais próximo de 0 maior)
	NVIC_EnableIRQ(BUT_PIO_ID);
	NVIC_SetPriority(BUT_PIO_ID, 4); // Prioridade 4
	
	NVIC_EnableIRQ(BUT2_PIO_ID);
	NVIC_SetPriority(BUT2_PIO_ID, 3); // Prioridade 3
	
	NVIC_EnableIRQ(BUT3_PIO_ID);
	NVIC_SetPriority(BUT3_PIO_ID, 5); // Prioridade 5
}


/************************************************************************/
/* Main                                                                 */
/************************************************************************/

// Funcao principal chamada na inicalizacao do uC.
void main(void)
{
	// Init OLED
	gfx_mono_ssd1306_init();
	// Inicializa clock
	sysclk_init();

	// Desativa watchdog
	WDT->WDT_MR = WDT_MR_WDDIS;
	
	int acende = 0;

	// configura botao com interrupcao
	io_init();
	// super loop
	// aplicacoes embarcadas no devem sair do while(1).
	atualiza_display();
	while(1){
		if(but_flag) {  //apertei?
			//espero pra verificar se foi longoou curto
			delay_ms(300);
			
			while(but_flag){ //ainda estou apertando?
				//if (t <900){ //ver se fica muito devagar
				t += 100; // intervalo aumenta
				atualiza_display();
				limpa_barra();
				delay_ms(150);
				//}
			}
			
			
			if (but_flag == 0) {  //soltei?
				if (t>100){ // o intervalo entre as piscadas será nulo?
					t -= 100; // intervalo diminuido
				}
				atualiza_display();
				limpa_barra();
				acende = 1;
			}
		}
		if(but_flag3){
			but_flag3 = 0;
			t += 100; //diminui a freq
			atualiza_display();
			acende= 1;
		}
		if(acende){
			limpa_barra();
			blink_led();
			acende = 0;
		}
		pmc_sleep(SAM_PM_SMODE_SLEEP_WFI); //Código 'trava' aqui até ser 'acordado'
	}
}