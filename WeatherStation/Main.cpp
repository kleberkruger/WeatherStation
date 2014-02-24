/*
 =======================================================================================================================
 File       : Main.cpp
 -----------------------------------------------------------------------------------------------------------------------
 Author     : Fábio Iaione
 Date       : 2013-03-13
 Version    : 1.0
 Copyright  : Faculty of Computing, FACOM - UFMS
 -----------------------------------------------------------------------------------------------------------------------
 Description: Weather station without implementing fault tolerance
 =======================================================================================================================
 */

#include "mbed.h"

#include "EthernetPowerControl.h"
#include "GPS.h"
#include "nRF24L01P.h"
#include "SHTx/sht15.hpp" 	// * Copyright (c) 2010 Roy van Dam <roy@negative-black.org> All rights reserved.#define NUM_SER     		123456#define CONST_VBAT  		15.085714286    // = 3,3V*(Rinf+Rsup)/Rinf#define CONV_ANEM   		1               // Constante para converter tempo do pulso em ms para velocidade do vento (m/s)#define INC_PLUV    		0.254           // Valor em mm de um pulso do pluviômetro#define NUM_PARAM   		9#define INTER_LEITURA   	10          	// Esse valor deve ser > 1#define UNID_LEITURA    	's'         	// 'm' para mim e 's' para seg#define HORA_TRANS      	11#define MIN_TRANS       	38#define HIST_SERIAL#define ERRO_LE_SENSORES    1#define ERRO_ABRE_ARQ       2#define SIM         		1#define NAO         		0#define TRANS       		1#define LER         		0using namespace std;
typedef union { // União para acessar os mesmos 4B de memória como float e como long

	float f;
	long l;
} float_long;

struct srt_registro {
	time_t data_hora;
	float_long param[NUM_PARAM];
	float_long crc;
};

void erro_fatal(char);
char conf_inicial(void);
char le_sensores(srt_registro *);
char armazena(srt_registro *);
char transmite(void);
void func_periodica(void);
void historico(const char *);
char verif_hora(char, char, int);
float le_sensor(float, float, float, float, char, char);
void conf_RTC(void);
char le_gps(void);

/**
 * Create the InterruptIn on the pin specified to Counter.
 * Modificada por Fábio Iaione para acrescentar debounce.
 */
class Counter {
public:

	Counter(PinName pin, char tipo_trans, PinMode pinM, unsigned int tmp_est_a) :
			_interrupt(pin), pino(pin) {

		if (tipo_trans == 'R') {
			_interrupt.rise(this, &Counter::agenda); // Attach increment function of this counter instance
			nivel_est = 1;
		}
		if (tipo_trans == 'F') {
			_interrupt.fall(this, &Counter::agenda); // Attach increment function of this counter instance
			nivel_est = 0;
		}

		_interrupt.mode(pinM);
		tmp_est = tmp_est_a;
		//_count = 0;
	}

	void agenda() {
		timeout.attach_us(this, &Counter::conta, tmp_est);
	}

	void conta() {
		if (pino == nivel_est) {
			_count++;
			LPC_RTC->GPREG0 = _count;
		}
	}

	int read() {
		_count = LPC_RTC->GPREG0;
		return _count;
	}

	void reset() {
		_count = 0;
		LPC_RTC->GPREG0 = _count;
	}

private:

	InterruptIn _interrupt;
	Timeout timeout;
	DigitalIn pino;
	volatile int _count;
	unsigned int tmp_est; 	// Tempo de estabilização em us
	char nivel_est; 		// NL de estabilização (1 para Rise, 0 para Fall )
};

/**
 * Criada por Fábio Iaione.
 */
class Molhamento {
public:

	Molhamento(PinName pin1, PinName pin2, PinName pin3) :
			pin_ad(pin1), pin_s(pin2), pin_i(pin3) {
		pin_s = 0;
		pin_i = 0;
	}

	void config(float res, int time, float rmin, float rmax) {
		r = res * 1000;
		t = time / 2;
		r_min = rmin * 1000;
		r_max = rmax * 1000;
	}

	float read() {

		float val_mol, lcad;

		pin_s = 1;
		wait_us(t);
		lcad = pin_ad.read();
		wait_us(t);
		pin_s = 0;
		pin_i = 1;
		wait_us(t);
		pin_ad.read();
		wait_us(t);
		pin_i = 0;
		val_mol = lcad * r / (1.0 - lcad);

		if (val_mol < r_min || lcad == 0.0)
			return (-INFINITY);
		if (val_mol > r_max || lcad == 1.0)
			return (INFINITY);

		return val_mol;
	}

private:

	AnalogIn pin_ad;
	DigitalOut pin_s, pin_i;
	float r, r_min, r_max;
	int t;
};

/**
 * Criada por Fábio Iaione.
 *
 * nome_objeto.config(NL_medido(L ou H), timeout_ms(retorna 0); tempo_estabilizar_repique_ms);
 * nome_objeto.read(): retorna o tempo de duração em ms
 */
class PulseIn2 {
public:

	PulseIn2(PinName pin1) :
			pin_in(pin1) {
	}

	void config(char measure_level, unsigned int timeout_ms, int tmp_est_ms) {
		m_l = measure_level;
		t_o = timeout_ms * 1000;
		t_e = tmp_est_ms;
	}

	void start(void) {

		t.start();
		wait_ms(t_e);

		if (m_l == 'L') {
			pin_in.rise(this, &PulseIn2::stop);
			pin_in.fall(NULL);
		} else {
			pin_in.fall(this, &PulseIn2::stop);
			pin_in.rise(NULL);
		}
	}

	void stop(void) {
		t.stop();
		t_out.detach();
		tme = t.read_ms();
		pin_in.rise(NULL);
		pin_in.fall(NULL);
	}

	void timeout_isr(void) {
		tme = 0;
		t_out.detach();
		pin_in.rise(NULL);
		pin_in.fall(NULL);
	}

	int read(void) {

		t.reset();
		tme = -1;

		if (m_l == 'L') {
			pin_in.mode(PullUp);
			pin_in.fall(this, &PulseIn2::start);
		} else {
			pin_in.mode(PullDown);
			pin_in.rise(this, &PulseIn2::start);
		}

		t_out.attach_us(this, &PulseIn2::timeout_isr, t_o);

		while (tme < 0)
			wait_ms(1); // Sem esse wait, não funciona

		return tme;
	}

private:

	InterruptIn pin_in;
	Timer t;
	Timeout t_out;
	int tme, t_e;
	unsigned int t_o;
	char m_l;
};

/**
 * Simon's Watchdog.
 * Code from: <http://mbed.org/forum/mbed/topic/508/>
 */
class Watchdog {
public:

	/**
	 * Load timeout value in watchdog timer and enable.
	 */
	void kick(float s) {
		LPC_WDT->WDCLKSEL = 0x1; 				// Set CLK src to PCLK
		uint32_t clk = SystemCoreClock / 16;	// WD has a fixed /4 prescaler, PCLK default is /4
		LPC_WDT->WDTC = s * (float) clk;
		LPC_WDT->WDMOD = 0x3; 					// Enabled and Reset
		kick();
	}

	/**
	 * "kick" or "feed" the dog - reset the watchdog timer
	 * by writing this required bit pattern.
	 */
	void kick() {
		LPC_WDT->WDFEED = 0xAA;
		LPC_WDT->WDFEED = 0x55;
	}
};

#ifdef HIST_SERIAL
Serial pc(USBTX, USBRX); // tx, rx
#endif
DigitalOut WDI(p23), LDBATT(p24), led1(LED1), led2(LED2), led3(LED3), led4(LED4);
Watchdog wdt;
Ticker weak;
Counter pluv(p22, 'F', PullUp, 5000); // Nome do pino, (R)ise ou (F)all, PullUp PullDown ou PullNone, tempo estabilização em us
DigitalOut ld_gps(p9);
GPS gps(p13, p14);

int main() {

	srt_registro registro;
	LocalFileSystem local("local");
	FILE *fp;

	erro_fatal(conf_inicial());

	while (1) {

		if (verif_hora(LER, UNID_LEITURA, INTER_LEITURA) == SIM) {
			erro_fatal(le_sensores(&registro));
			erro_fatal(armazena(&registro));
			fp = fopen("/local/pronto.txt", "w"); // Se o arquivo existir, grava por cima

			if (fp) {
				fprintf(fp, "PRONTO");
				fclose(fp);
			}
		}

		if (verif_hora(TRANS, 0, 0) == SIM)
			erro_fatal(transmite());
		if (UNID_LEITURA == 'm')
			Sleep();
		else
			wait(0.5);
	}
}

char conf_inicial(void) {

	if ((LPC_WDT->WDMOD >> 2) & 1) {

		// Reset por watchdog
		historico(">>>> conf_inicial(): Reset por watchdog.");
		led3 = 1;
		wait(0.5);
		led3 = 0;

	} else {

		// Reset por push-button ou power-on
		historico(">>>> conf_inicial(): Reset por push-button ou power on.");
		LocalFileSystem local("local");

		FILE *fp = fopen("/local/conf.txt", "w"); // Se o arquivo existir, grava por cima
		if (!fp)
			erro_fatal (ERRO_ABRE_ARQ);

		weak.attach(&func_periodica, 5.0); 	// Função func_periodoca chamada a cada 5s
		wdt.kick(6.0); 						// Tempo de 6s do watchdog
		WDI = 1; 							// Desliga mbed e Bat
		LDBATT = 1;
		wait(0.5);
		WDI = 0; 							// Liga mbed
		led1 = 1;
		wait(0.1);
		led1 = 0;
		ld_gps = 0; 						// Desabilita GPS
		PHY_PowerDown(); 					// Desativa ethernet para reduzir consumo
		pluv.reset(); 						// Zera o contador de pulsos recebidos do pluviômetro
		conf_RTC(); 						// Sincronizar RTC e outras coisas com o site

		fprintf(fp, "Parametro \t Unidade \t Valor minimo valido \t Valor maximo valido\n");
		fprintf(fp, "Temperatura do ar \t graus C \t ? \t ?\n");
		fprintf(fp, "Umidade do ar \t %% U.R. \t ? \t ?\n");
		fprintf(fp, "Pluviometria \t mm \t 0.0 \t ?\n");
		fprintf(fp, "Velocidade do vento \t m/s \t 0.0 \t ?\n");
		fprintf(fp, "Umidade do solo \t Raiz de epsilon \t 1.1 \t 5.54\n");
		fprintf(fp, "Temperatura do solo \t graus C \t 5.0 \t 50.0\n");
		fprintf(fp, "Irradiacao solar \t W/m2 \t 0.0 \t 1500\n");
		fprintf(fp, "Molhamento foliar \t kohms \t 1.0 \t 3000\n");
		fprintf(fp, "Tensao da bateria \t Volts \t 0.0 \t 15.0\n");
		fprintf(fp, "\nNumero de serie= \t %d\n", NUM_SER);
		fprintf(fp, "Versao do firmware= \t %s \t %s \t %s\n", __TIME__, __DATE__, __FILE__);

//		if (le_gps()) fprintf(fp, "Latitude= \t %f \t Longitude= \t %f\n", gps.latitude, gps.longitude);
//		else fprintf(fp, "Latitude= \t ? \t Longitude= \t ?\n");

		fclose(fp);
	}

	return 0;
}

void erro_fatal(char num_erro) {
	char cont;

	if (num_erro == 0)
		return;

	while (1) {

		for (cont = 0; cont < 6; cont++) {
			led2 = 1;
			wait(0.05);
			led2 = 0;
			wait(0.05);
		}

		for (cont = 0; cont < num_erro; cont++) {
			led2 = 0;
			wait(1.0);
			led2 = 1;
			wait(0.1);
			led2 = 0;
			wait(1.0);
		}
	}
}

char verif_hora(char tipo, char unid, int inter_leit) {
	struct tm * timest;
	time_t time_sec;
	static char flag_ler = NAO;
	static char flag_trans = NAO;
	int valtm;

//	historico("verif_hora() iniciada");

	time(&time_sec);
	timest = localtime(&time_sec);

	switch (tipo) {

		case LER:
			if (unid == 'm') {
				valtm = timest->tm_min;
			} else {
				valtm = timest->tm_sec;
			}
			if ((valtm % inter_leit) == 0) {
				if (flag_ler == NAO) {
					flag_ler = SIM;
					return SIM;
				} else
					return NAO;
			} else {
				flag_ler = NAO;
				return NAO;
			}

		case TRANS:
			if ((timest->tm_min == MIN_TRANS) && (timest->tm_hour == HORA_TRANS)) {
				if (flag_trans == NAO) {
					flag_trans = SIM;
					return SIM;
				} else
					return NAO;
			} else {
				flag_trans = NAO;
				return NAO;
			}

		default:
			return NAO;
	}
}

void historico(const char * pont_texto) {
	LocalFileSystem local("local");
	char buffer[32];

	FILE *fp = fopen("/local/histor.txt", "a");
	if (!fp)
		erro_fatal (ERRO_ABRE_ARQ);

	time_t seconds = time(NULL);

	strftime(buffer, 32, "%d/%m/%Y %H:%M:%S", localtime(&seconds));
#ifdef HIST_SERIAL
	pc.printf("%s >>> %s\n", buffer, pont_texto);
#endif
	fprintf(fp, "%s >>> %s\n", buffer, pont_texto);
	fclose(fp);
}

void func_periodica(void) {
//	historico("func_periodica() iniciada");
	wdt.kick();
	led4 = 1;
	wait(0.03);
	led4 = 0;
}

char le_sensores(srt_registro * pt) {

	char cont;

	LDBATT = 0; 	// Liga Vbat e 5Vc
	wait_ms(200); 	// Tempo para Vbat estabilizar, pois o acionamento do MOSFET é lento (+/- 23ms)

	historico("le_sensores() iniciada");

	SHTx::SHT15 sensorTE_UR(p29, p30); 	// DATA, SCK
	sensorTE_UR.setOTPReload(false);
	sensorTE_UR.setResolution(true);
	PulseIn2 vel(p21);
	vel.config('L', 500, 5); 			// nome_objeto.config(NL_medido(L ou H), timeout_ms(retorna 0); tempo_estabilizar_repique_ms);
	Molhamento mol(p19, p26, p25);
	mol.config(100, 1000, 1, 3000); 	// 100kohms, 1000us, 1kohms, 3000k=3Mohms

	pt->data_hora = time(NULL);

	sensorTE_UR.update();
	sensorTE_UR.setScale(false);

	pt->param[0].f = sensorTE_UR.getTemperature();
	pt->param[1].f = sensorTE_UR.getHumidity();
	pt->param[2].f = pluv.read() * INC_PLUV;
	pt->param[3].f = vel.read() * CONV_ANEM;
	pt->param[4].f = le_sensor(1.1, 0, 5.54, 1.0, 16, 0); 				// Umidade do solo [raiz de epsilon]
	pt->param[5].f = le_sensor(5, 0.320512821, 50, 3.205128205, 17, 0); // Temperatura do solo [C]
	pt->param[6].f = le_sensor(0, 0, 1500, 1.5, 18, 0); 				// Irradiação solar [W/m2]
	pt->param[7].f = mol.read() / 1000; 								// Molhamento [kohms]
	pt->param[8].f = le_sensor(0, 0, 15.085714286, 3.3, 15, 0); 		// Tensão da bateria [V]

	pt->crc.l = pt->data_hora; // Calcula CRC

	for (cont = 0; cont < NUM_PARAM; cont++)
		pt->crc.l = pt->crc.l ^ pt->param[cont].l;

	LDBATT = 1; // Desliga Vbat e 5Vc

	historico("le_sensores() concluida");

	return 0;
}

char armazena(srt_registro * pt) {
	historico("armazena() iniciada");
	LocalFileSystem local("local");

	FILE *fp = fopen("/local/dados.dat", "ab");
	if (!fp)
		return ERRO_ABRE_ARQ;

	fwrite(pt, sizeof(struct srt_registro), 1, fp);
	fclose(fp);

	historico("armazena() concluida");
	return 0;
}

char transmite(void) {
	historico("transmite() iniciada: nao implementada ainda");
	return 0;
}

float le_sensor(float v_ini_par, float v_ini_volts, float v_fim_par, float v_fim_volts, char num_pino, char tipo_ret) {

	AnalogIn ea[] = { (p15), (p16), (p17), (p18), (p19), (p20) };
	float ca, cl, par, lcad;

	ca = (v_fim_par - v_ini_par) / (v_fim_volts - v_ini_volts);
	cl = v_ini_par - ca * v_ini_volts;
	lcad = ea[num_pino - 15].read();
	par = ca * lcad * 3.3 + cl;

	if (ca > 0) {
		if (par < v_ini_par || lcad == 0.0)
			return (-INFINITY);
		if (par > v_fim_par || lcad == 1.0)
			return (INFINITY);
	}

	if (ca < 0) {
		if (par > v_ini_par || lcad == 0.0)
			return (INFINITY);
		if (par < v_fim_par || lcad == 1.0)
			return (-INFINITY);
	}

	return (par);
}

void conf_RTC(void) {
	struct tm t;
	Timer tpo;
	int ano, mes;

	pc.printf("Acertar RTC? (qualquer tecla = sim)\n");
	tpo.start();

	while (!pc.readable() && tpo.read() < 2);

	if (pc.readable()) {

		while (pc.getc() != 0x0D);

		pc.printf("Entre com dd/mm/aaaa hh:mm\n");
		t.tm_sec = 0;
		pc.scanf("%d/%d/%d %d:%d", &t.tm_mday, &mes, &ano, &t.tm_hour, &t.tm_min);
		t.tm_year = ano - 1900;
		t.tm_mon = mes - 1;
		set_time(mktime(&t));
	}
//	else set_time(1256729737); // 28 Oct 2009 11:35:37
}

char le_gps(void) {

	Timer tpo;

	ld_gps = 1; // Habilita GPS
	tpo.start();

	do {

		if (gps.sample()) {

//			pc.printf("\ngetline(): %s\n", gps.msg);
			historico("Lock Ok");
			ld_gps = 0; // Desabilita GPS

			return 1;

		} else {
			historico("No lock.");
		}

		wait(1.0);

	} while (!gps.lock && tpo.read() < 3);

	ld_gps = 0; // Desabilita GPS

	return 0;
}
