/*
 * NVM.c
 *
 *  Created on: 20 Apr 2020
 *      Author: aangelino
 */
#include <NVM.h>
#include "string.h"
#include "flash.h"
#include "Processes.h"

extern OvenType currOvenType;
extern SHORT usSRegInBuf[];
//static uint8_t s_nvmData[(((sizeof(sNvmStorage_t) + 7u) / 8u) * 8u)] __attribute__((aligned (8)));

/*current running NVM values*/
sNvmStorage_t currNvm;

static uint8_t s_nvmData[(((sizeof(currNvm) + 7u) / 8u) * 8u)] __attribute__((aligned (8)));
/*default NVM values*/
static const sNvmStorage_t defaultNvm=
{
		.A={
				0, 		/*0*< A01, Tempo black-out ammissibile, sec */
				50, 	/*1*< A02, MIN set-point Cottura Convezione, °F */
				518, 	/*2*< A03, MAX set-point Cottura Convezione, °F */
				86, 	/*3*< A04, MIN set-point Cottura Mista, °F */
				482, 	/*4*< A05, MAX set-point Cottura Mista, °F */
				86, 	/*5*< A06, MIN set-point Cottura Vapore, °F */
				266, 	/*6*< A07, MAX set-point Cottura Vapore, °F */
				54, 	/*7*< A08, Over Range Convezione, °F */
				347, 	/*8*< A10, 1a Soglia H2O scarico, °F */
				158, 	/*9*< A11, 2a Soglia H2O scarico, °F */
				149, 	/*10*< A12, 3a Soglia H2O scarico, °F */
				60, 	/*11*< A13, T-on H2O scarico - riempimento sifone, sec */
				5, 		/*12*< A14, T-on comando per apertura sfiato, sec */
				240, 	/*13*< A16, Tempo di rotaz. oraria motore, sec */
				240, 	/*14*< A17, Tempo di rotaz. antioraria motore, sec */
				12, 	/*15*< A18, Tempo Stop motore per inversione, sec */
				149, 	/*16*< A19, MIN.Temperat. Abilitaz. Inversione Motore, °F */
				2, 		/*17*< A20, Ritardo riscaldamento sullo START motori, sec */
				5, 		/*18*< A21, Tempo min. sicurezza riaccensione, sec */
				3, 		/*19*< A22, Tempo min. sicurezza rispegnimento, sec */
				2, 		/*20*< A23, Correzione set-point riscaldamento, °F */
				6, 		/*21*< *A24, N.B da /10 (0.6) Correz. Set-point riscaldam. 1^ montata, °F */
				2, 		/*22*< *A25, N.B da /10 (0.2) Isteresi accens-spegnimento riscaldam., °F */
				3, 		/*23*< A26, Coefficiente derivativo prima "montata", °F */
				4, 		/*24*< A27, Conferma lettura allarmi permanenti, sec */
				151, 	/*25*< A28, Soglia Allarme temperat.Scheda quadro, °F */
				155, 	/*26*< A29, Soglia STOP temperat.Scheda quadro, °F */
				20, 	/*27*< A32, Contatore verifica apertura porta */
				1, 		/*28*< A33, Abilitaz. (DO-2) e/o (DO-3) all'invers.motori */
				1, 		/*29*< A34, Soglia temporizz.H20 camera Mod.Vapore, °F */
				10, 	/*30*< A35, Periodo H20 camera Modo Vapore, sec */
				7, 		/*31*< A36, T-on H20 camera Modo Vapore, sec */
				40, 	/*32*< A37, Periodo H20 scarico, sec */
				30, 	/*33*< A38, T-on H20 scarico 1a soglia, sec */
				6, 		/*34*< A39, T-on H20 scarico 2a soglia, sec */
				0, 		/*35*< A40, T-on H20 scarico 3a soglia, sec */
				2, 		/*36*< A41, RITARDO APERTURA SFIATO */
				1, 		/*37*< A57, Legame (DO-5) 0=no 1=si */
				212, 	/*38*< A59, Soglia cotture a bassa temperatura, °F */
				44, 	/*39*< *A60, N.B da /10 (4.4) Correzione set-point riscaldam. Btemp, °F */
				3, 		/*40*< A61, Correz. Set-point riscaldam. 1^ mont Btemp, °F */
				64, 	/*41*< *A62, N.B da /10 (6.4) Coeff. derivativo prima montata Btemp, °F */
				446, 	/*42*< A63, Soglia cotture alta temperatura, °F */
				212, 	/*43*< A65, Tset max per cotture sottovuoto, °F */
				7, 		/*44*< A66, Periodo H2O sottovuoto Vapore, sec */
				2, 		/*45*< A67, Ton H2O sottovuoto Vapore, sec */
				9, 		/*46*< A68, Periodo H2O per A57=1, sec */
				4, 		/*47*< A69, Ton H2O per A57=1, sec */
				60, 	/*48*< A70, Tempo OFF ventola forno statico, sec */
				16, 	/*49*< A71, Tempo ON ventola forno statico, sec */
				100, 	/*50*< A79, T-set default raffreddamento camera, °C */
				20, 	/*51*< A84, Tempo spegnim. Luci porta chiusa, sec */
				1, 		/*52*< A87, Delta da aggiungere a A36, sec */
				3, 		/*53*< A88, Delta da aggiungere a A69, sec */
				12, 	/*54*< A89, Tempo limite di pulizia forno per L3, hour */
				1, 		/*55*< A90, Abilitazione tempo limite pulizia forno per L3 */
				15, 	/*56*< A91, Tempo di immissione acqua in camera per decalcificazione, hour */
				1, 		/*57*< A92, Abilitazione tempo di immissione acqua per decalcificazione */
				10, 	/*58*< A93, Tempo di ciclo per T<Tset-A34, sec */
				7, 		/*59*< A94, Tempo di ON per T<Tset-A34, sec */
				9, 		/*60*< A97, Tempo di ciclo per T>Tset-A34, sec */
				2, 		/*61*< A98, Tempo di ON per T>Tset-A34, sec */
				10, 	/*62*< A104, Periodo H20 camera Modo STANDARD Vapore (flussostato), sec */
				46, 	/*63*< A105, CNT H20 camera Modo STANDARD Vapore (flussostato) */
				10, 	/*64*< A106, Periodo H2O STANDARD per A57=1 (flussostato), sec */
				23, 	/*65*< A107, CNT H2O camera modo STANDARD per A57=1 (flussostato) */
				10, 	/*66*< A108, Periodo H20 camera Modo SUPER Vapore (flussostato), sec */
				63, 	/*67*< A109, CNT H20 camera Modo SUPER Vapore (flussostato) */
				10, 	/*68*< A110, Periodo H2O SUPER per A57=1 (flussostato), sec */
				63, 	/*69*< A111, CNT H2O camera modo SUPER per A57=1 (flussostato) */
				10, 	/*70*< A112, Periodo H20 camera Modo SMALL Vapore (flussostato), sec */
				46, 	/*71*< A113, CNT H20 camera Modo SMALL Vapore (flussostato) */
				10, 	/*72*< A114, Periodo H2O SMALL per A57=1 (flussostato), sec */
				13	 	/*73*< A115, CNT H2O camera modo SMALL per A57=1 (flussostato) */
		},

		.AA={
				29,			/*0*< AA01, Soglia rampa di salita in MISTO, % */
				29,			/*1*< AA02, Soglia rampa di discesa in MISTO, % */
				3,			/*2*< AA03, Tempo di ON relay 2 per rampa di salita, sec */
				3,			/*3*< AA04, Tempo di OFF relay 2 per rampa di salita, sec */
				60,			/*4*< AA05, Tempo di attuazione rampa di salita, sec */
				30,			/*5*< AA06, Apertura valvola sfiato per rampa di discesa, sec */
				52,			/*6*< AA29, Delta T1 rampa negativa, °F */
				1,			/*7*< AA30, Delta T2 rampa negativa, °F */
				1,			/*8*< AA41, Tempo di sfiato aperto per piccole rampe negative, sec */
				3,			/*9*< AA42, H2O on per rampe negative sotto AA44, sec */
				3,			/*10*< AA43, H2O off per rampe negative sotto AA44, sec */
				203,		/*11*< AA44, Soglia di attivazione per AA42, AA43, sec */
				0,			/*12*< AA45, Abilitazione cooldown per convezione secco */
				0,			/*13*< AA46, Abilitazione cooldown per convezione vapore */
				0,			/*14*< AA47, Abilitazione cooldown per misto */
				0,			/*15*< AA48, Abilitazione cooldown per vapore */
				5,			/*16*< AA49, Anticipo apertura sfiato per inizio rampe negative di temperatura, sec */
				5,			/*17*< AA50, Ritardo chiusura sfiato per fine rampe negative di temperatura, sec */
				50,			/*18*< AA53, Delta T3 Cool-down, °F */
				212,		/*19*< AA56, Soglia di immiss.H2O modo conservazione, °F */
				9,			/*20*< AA57, Delta 1 Temp. camera rispetto Temp.cuore modo conservazione per effetto volano, °F */
				5,			/*21*< AA58, Delta 2 Temp. camera rispetto Temp.cuore per conservazione, °F */
				10,			/*22*< AA59, Timeout per start ventola in conservazione, min */
				1439,		/*23*< AA60, Tempo massimo di conservazione, min (23h 59min da tabella)*/
				482,		/*24*< AA72, Temperatura di immissione H2O in cooldown, °F */
				4,			/*25*< AA74, Cadenza Impulsi al sec di conteggio minimi, imp/sec */
				0,			/*26*< AA75, Tipo di forno */
				1,			/*27*< AA76, Abilitazione allarmi E17a e E17c */
				1			/*28*< AA77, Fattore correttivo impulsi */
		},

		.M={
				0, 	/**< offset value*/
				1,	/**< M1*/
				14,	/**< M2*/
				1,	/**< M3*/
				14,	/**< M4*/
				1,	/**< M5*/
				14,	/**< M6*/
				2,	/**< M7*/
				14,	/**< M8*/
				2,	/**< M9*/
				14,	/**< M10*/
				2,	/**< M11*/
				14,	/**< M12*/
				2,	/**< M13*/
				14,	/**< M14*/
				3,	/**< M15*/
				14,	/**< M16*/
				3,	/**< M17*/
				14,	/**< M18*/
				6,	/**< M19*/
				15,	/**< M20*/
				6,	/**< M21*/
				15,	/**< M22*/
				6,	/**< M23*/
				15,	/**< M24*/
				12,	/**< M25*/
				16,	/**< M26*/
				12,	/**< M27*/
				16,	/**< M28*/
				12,	/**< M29*/
				16,	/**< M30*/
				12,	/**< M31*/
				16,	/**< M32*/
				17,	/**< M33*/
				17,	/**< M34*/
				17,	/**< M35*/
				17,	/**< M36*/
				8,	/**< M37*/
				45,	/**< M38*/
				8,	/**< M39*/
				45,	/**< M40*/
				8,	/**< M41*/
				45,	/**< M42*/
				8,	/**< M43*/
				45,	/**< M44*/
				8,	/**< M45*/
				45,	/**< M46*/
				8,	/**< M47*/
				45,	/**< M48*/
				8,	/**< M49*/
				45,	/**< M50*/
				8,	/**< M51*/
				45,	/**< M52*/
				8,	/**< M53*/
				45,	/**< M54*/
				3,	/**< M55*/
				50,	/**< M56*/
				6,	/**< M57*/
				50,	/**< M58*/
				9,	/**< M59*/
				50,	/**< M60*/
				9,	/**< M61*/
				31,	/**< M62*/
				9,	/**< M63*/
				20,	/**< M64*/
				12,	/**< M65*/
				17,	/**< M66*/
				12,	/**< M67*/
				17,	/**< M68*/
				12,	/**< M69*/
				17,	/**< M70*/
				12,	/**< M71*/
				17,	/**< M72*/
				17,	/**< M73*/
				53,	/**< M74*/
				34,	/**< M75*/
				56,	/**< M76*/
				52,	/**< M77*/
				59,	/**< M78*/
				52,	/**< M79*/
				40,	/**< M80*/
				52,	/**< M81*/
				29,	/**< M82*/
				69,	/**< M83*/
				29,	/**< M84*/
				69,	/**< M85*/
				29,	/**< M86*/
				69,	/**< M87*/
				29,	/**< M88*/
				69,	/**< M89*/
				29,	/**< M90*/
				8,	/**< M91*/
				60,	/**< M92*/
				8,	/**< M93*/
				60,	/**< M94*/
				8,	/**< M95*/
				60,	/**< M96*/
				8,	/**< M97*/
				60,	/**< M98*/
				8,	/**< M99*/
				60,	/**< M100*/
				8,	/**< M101*/
				60,	/**< M102*/
				8,	/**< M103*/
				105,/**< M104*/
				8,	/**< M105*/
				155,/**< M106*/
				8,	/**< M107*/
				199,/**< M108*/
				3,	/**< M109*/
				50,	/**< M110*/
				6,	/**< M111*/
				50,	/**< M112*/
				9,	/**< M113*/
				50,	/**< M114*/
				9,	/**< M115*/
				31,	/**< M116*/
				9,	/**< M117*/
				20,	/**< M118*/
				12,	/**< M119*/
				17,	/**< M120*/
				12,	/**< M121*/
				17, /**< M122*/
				12,	/**< M123*/
				17, /**< M124*/
				12,	/**< M125*/
				17, /**< M126*/
				17,	/**< M127*/
				53,	/**< M128*/
				34,	/**< M129*/
				56,	/**< M130*/
				52,	/**< M131*/
				59,	/**< M132*/
				52,	/**< M133*/
				40,	/**< M134*/
				52,	/**< M135*/
				29,	/**< M136*/
				69,	/**< M137*/
				29,	/**< M138*/
				69,	/**< M139*/
				29, /**< M140*/
				69,	/**< M141*/
				29, /**< M142*/
				69,	/**< M143*/
				29, /**< M144*/
				8,	/**< M145*/
				60,	/**< M146*/
				8,	/**< M147*/
				60,	/**< M148*/
				8,	/**< M149*/
				60,	/**< M150*/
				8,	/**< M151*/
				60,	/**< M152*/
				8,	/**< M153*/
				60,	/**< M154*/
				8,	/**< M155*/
				60,	/**< M156*/
				8,	/**< M157*/
				105,/**< M158*/
				8,	/**< M159*/
				155,/**< M160*/
				8,	/**< M161*/
				199,/**< M162*/
				3,	/**< M163*/
				50,	/**< M164*/
				6,	/**< M165*/
				50,	/**< M166*/
				9,	/**< M167*/
				50,	/**< M168*/
				9,	/**< M169*/
				31,	/**< M170*/
				9,	/**< M171*/
				20,	/**< M172*/
				12,	/**< M173*/
				17,	/**< M174*/
				12,	/**< M175*/
				17, /**< M176*/
				12,	/**< M177*/
				17, /**< M178*/
				12,	/**< M179*/
				17, /**< M180*/
				17,	/**< M181*/
				53,	/**< M182*/
				34,	/**< M183*/
				56,	/**< M184*/
				52,	/**< M185*/
				59,	/**< M186*/
				52,	/**< M187*/
				40,	/**< M188*/
				52,	/**< M189*/
				29,	/**< M190*/
				69,	/**< M191*/
				29,	/**< M192*/
				69,	/**< M193*/
				29, /**< M194*/
				69,	/**< M195*/
				29, /**< M196*/
				69,	/**< M197*/
				29, /**< M198*/
				8,	/**< M199*/
				60,	/**< M200*/
				8,	/**< M201*/
				60,	/**< M202*/
				8,	/**< M203*/
				60,	/**< M204*/
				8,	/**< M205*/
				60,	/**< M206*/
				8,	/**< M207*/
				60,	/**< M208*/
				8,	/**< M209*/
				60,	/**< M210*/
				8,	/**< M211*/
				105, /**< M212*/
				8,	/**< M213*/
				155, /**< M214*/
				8,	/**< M215*/
				199, /**< M216*/
				3,	/**< M217*/
				50,	/**< M218*/
				6,	/**< M219*/
				50,	/**< M220*/
				9,	/**< M221*/
				50,	/**< M222*/
				9,	/**< M223*/
				31,	/**< M224*/
				9,	/**< M225*/
				20,	/**< M226*/
				12,	/**< M227*/
				17,	/**< M228*/
				12,	/**< M229*/
				17, /**< M230*/
				12,	/**< M231*/
				17, /**< M232*/
				12,	/**< M233*/
				17, /**< M234*/
				17,	/**< M235*/
				53,	/**< M236*/
				34,	/**< M237*/
				56,	/**< M238*/
				52,	/**< M239*/
				59,	/**< M240*/
				52,	/**< M241*/
				40,	/**< M242*/
				52,	/**< M243*/
				29,	/**< M244*/
				69,	/**< M245*/
				29,	/**< M246*/
				69,	/**< M247*/
				29, /**< M248*/
				69,	/**< M249*/
				29, /**< M250*/
				69,	/**< M251*/
				29, /**< M252*/
				8,	/**< M253*/
				60,	/**< M254*/
				8,	/**< M255*/
				60,	/**< M256*/
				8,	/**< M257*/
				60,	/**< M258*/
				8,	/**< M259*/
				60,	/**< M260*/
				8,	/**< M261*/
				60,	/**< M262*/
				8,	/**< M263*/
				60,	/**< M264*/
				8,	/**< M265*/
				105,/**< M266*/
				8,	/**< M267*/
				155,/**< M268*/
				8,	/**< M269*/
				199,/**< M270*/
				3,	/**< M271*/
				50,	/**< M272*/
				6,	/**< M273*/
				50,	/**< M274*/
				9,	/**< M275*/
				50,	/**< M276*/
				9,	/**< M277*/
				31,	/**< M278*/
				9,	/**< M279*/
				20,	/**< M280*/
				12,	/**< M281*/
				17,	/**< M282*/
				12,	/**< M283*/
				17, /**< M284*/
				12,	/**< M285*/
				17, /**< M286*/
				12,	/**< M287*/
				17, /**< M288*/
				17, /**< M289*/
				53,	/**< M290*/
				34,	/**< M291*/
				56,	/**< M292*/
				52,	/**< M293*/
				59,	/**< M294*/
				52,	/**< M295*/
				40,	/**< M296*/
				52,	/**< M297*/
				29,	/**< M298*/
				69,	/**< M299*/
				29,	/**< M300*/
				69,	/**< M301*/
				29,	/**< M302*/
				69, /**< M303*/
				29,	/**< M304*/
				69, /**< M305*/
				29,	/**< M306*/
				8,  /**< M307*/
				60,	/**< M308*/
				8,	/**< M309*/
				60,	/**< M310*/
				8,	/**< M311*/
				60,	/**< M312*/
				8,	/**< M313*/
				60,	/**< M314*/
				8,	/**< M315*/
				60,	/**< M316*/
				8,	/**< M317*/
				60,	/**< M318*/
				8,	/**< M319*/
				105,/**< M320*/
				8,  /**< M321*/
				155,/**< M322*/
				8,  /**< M323*/
				199,/**< M324*/
				3,  /**< M325*/
				50,	/**< M326*/
				6,	/**< M327*/
				50,	/**< M328*/
				9,	/**< M329*/
				50,	/**< M330*/
				9,	/**< M331*/
				31,	/**< M332*/
				9,	/**< M333*/
				20,	/**< M334*/
				12,	/**< M335*/
				17,	/**< M336*/
				12,	/**< M337*/
				17, /**< M338*/
				12, /**< M339*/
				17, /**< M340*/
				12, /**< M341*/
				17, /**< M342*/
				17, /**< M343*/
				53,	/**< M344*/
				34,	/**< M345*/
				56,	/**< M346*/
				52,	/**< M347*/
				59,	/**< M348*/
				52,	/**< M349*/
				40,	/**< M350*/
				52,	/**< M351*/
				29,	/**< M352*/
				69,	/**< M353*/
				29,	/**< M354*/
				69,	/**< M355*/
				29, /**< M356*/
				69, /**< M357*/
				29, /**< M358*/
				69, /**< M359*/
				29, /**< M360*/
				8,  /**< M361*/
				60,	/**< M362*/
				8,	/**< M363*/
				60,	/**< M364*/
				8,	/**< M365*/
				60,	/**< M366*/
				8,	/**< M367*/
				60,	/**< M368*/
				8,	/**< M369*/
				60,	/**< M370*/
				8,	/**< M371*/
				60,	/**< M372*/
				8,	/**< M373*/
				105,/**< M374*/
				8,  /**< M375*/
				155,/**< M376*/
				8,  /**< M377*/
				199,/**< M378*/
				3,	/**< M379*/
				50,	/**< M380*/
				6,	/**< M381*/
				50,	/**< M382*/
				9,	/**< M383*/
				50,	/**< M384*/
				9,	/**< M385*/
				31,	/**< M386*/
				9,	/**< M387*/
				20,	/**< M388*/
				12,	/**< M389*/
				17,	/**< M390*/
				12,	/**< M391*/
				17, /**< M392*/
				12,	/**< M393*/
				17, /**< M394*/
				12,	/**< M395*/
				17, /**< M396*/
				17, /**< M397*/
				53,	/**< M398*/
				34,	/**< M399*/
				56,	/**< M400*/
				52,	/**< M401*/
				59,	/**< M402*/
				52,	/**< M403*/
				40,	/**< M404*/
				52,	/**< M405*/
				29,	/**< M406*/
				69,	/**< M407*/
				29,	/**< M408*/
				69,	/**< M409*/
				29,	/**< M410*/
				69, /**< M411*/
				29,	/**< M412*/
				69, /**< M413*/
				29,	/**< M414*/
				8,  /**< M415*/
				60,	/**< M416*/
				8,	/**< M417*/
				60,	/**< M418*/
				8,	/**< M419*/
				60,	/**< M420*/
				8,	/**< M421*/
				60,	/**< M422*/
				8,	/**< M423*/
				60,	/**< M424*/
				8,	/**< M425*/
				60,	/**< M426*/
				8,	/**< M427*/
				105,/**< M428*/
				8,  /**< M429*/
				155,/**< M430*/
				8,  /**< M431*/
				199,/**< M432*/
				3,  /**< M433*/
				50,	/**< M434*/
				6,	/**< M435*/
				50,	/**< M436*/
				9,	/**< M437*/
				50,	/**< M438*/
				9,	/**< M439*/
				31,	/**< M440*/
				9,	/**< M441*/
				20,	/**< M442*/
				12,	/**< M443*/
				17,	/**< M444*/
				12,	/**< M445*/
				17, /**< M446*/
				12, /**< M447*/
				17, /**< M448*/
				12, /**< M449*/
				17, /**< M450*/
				17, /**< M451*/
				53,	/**< M452*/
				34,	/**< M453*/
				56,	/**< M454*/
				52,	/**< M455*/
				59,	/**< M456*/
				52,	/**< M457*/
				40,	/**< M458*/
				52,	/**< M459*/
				29,	/**< M460*/
				69,	/**< M461*/
				29,	/**< M462*/
				69,	/**< M463*/
				29, /**< M464*/
				69, /**< M465*/
				29, /**< M466*/
				69, /**< M467*/
				29, /**< M468*/
				8,  /**< M469*/
				60,	/**< M470*/
				8,	/**< M471*/
				60,	/**< M472*/
				8,	/**< M473*/
				60,	/**< M474*/
				8,	/**< M475*/
				60,	/**< M476*/
				8,	/**< M477*/
				60,	/**< M478*/
				8,	/**< M479*/
				60,	/**< M480*/
				8,	/**< M481*/
				105,/**< M482*/
				8,  /**< M483*/
				155,/**< M484*/
				8,  /**< M485*/
				199/**< M486*/
		},

		.V={
				0, 	/*Speed 0*/
				200,	/*Speed 1*/
				300,	/*Speed 2*/
				440,	/*Speed 3*/
				470,	/*Speed 4*/
				500,	/*Speed 5*/
				482,	/*Prima Soglia   °F*/
				518		/*Seconda Soglia °F*/
		},
		//.I={0,1},
		.L={
				212, 	/*0*< L0, Temperatura di raffreddamento camera in lavaggio, °F */
				600, 	/*1*< L1, Tempo fase 1 lavaggio standard */
				500, 	/*2*< L2, Tempo fase 1 lavaggio medium */
				400, 	/*3*< L3, Tempo fase 1 lavaggio basic,  */
	 			194, 	/*4*< L4, Temperatura di termoregolazione lavaggio */
				60, 	/*5*< L5, Tempo attesa fasi 2, 5, 14, 23,  */
				60,		/*6*< L6, Tempo risciacquo fase 3 standard,  */
				50,		/*7*< L7, Tempo risciacquo fase 3 medium */
				40,		/*8*< L8, Tempo risciacquo fase 3 basic */
				360, 	/*9*< L10, Tempo ammollo a vapore fase 4, 13, 22 standard */
				240, 	/*10*< L11, Tempo ammollo a vapore fase 4, 13 medium */
				180, 	/*11*< L12, Tempo ammollo a vapore fase 4, 13 basic */
				15, 	/*12*< L13, Tempo erogazione Detergente fase 6, 8 standard taglia 6 */
				15, 	/*13*< L131, Tempo erogazione Detergente fase 6, 8 standard taglia 10 */
				10, 	/*14*< L14, Tempo erogazione Detergente fase 6, 8 medium taglia 6 */
				10, 	/*15*< L141, Tempo erogazione Detergente fase 6, 8 medium taglia 10*/
				8, 		/*16*< L15, Tempo erogazione Detergente fase 6, 8 basic taglia 6 */
				8, 		/*17*< L151, Tempo erogazione Detergente fase 6, 8 basic taglia 10 */
				20, 	/*18*< L16, Tempo avvio ventola fase 7 lavaggio */
				300, 	/*19*< L17, Tempo reazione fase 9, 18 standard */
				240, 	/*20*< L18, Tempo reazione fase 9, 18 medium */
				180, 	/*21*< L19, Tempo reazione fase 9, 18 basic */
				180, 	/*22*< L20, Tempo ammollo a vapore fase 10, 19, 26 */
				300, 	/*23*< L21, Tempo reazione fase 11, 20 standard */
				240, 	/*24*< L22, Tempo reazione fase 11, 20 medium */
				180, 	/*25*< L23, Tempo reazione fase 11, 20 basic */
				60, 	/*26*< L24, Tempo risciacquo fase 12, 21 standard */
				50, 	/*27*< L25, Tempo risciacquo fase 12, 21 medium */
				40, 	/*28*< L26, Tempo risciacquo fase 12, 21 basic */
				15, 	/*29*< L27, Tempo erogazione Detergente fase 15, 17 standard taglia 6 */
				15, 	/*30*< L271, Tempo erogazione Detergente fase 15, 17 standard taglia 10 */
				10, 	/*31*< L28, Tempo erogazione Detergente fase 15, 17 medium taglia 6 */
				10, 	/*32*< L281, Tempo erogazione Detergente fase 15, 17 medium taglia 10 */
				8, 		/*33*< L29, Tempo erogazione Detergente fase 15, 17 basic taglia 6 */
				8, 		/*34*< L291, Tempo erogazione Detergente fase 15, 17 basic taglia 10 */
				40, 	/*35*< L30, Tempo risciacquo fase 21 soft */
				240, 	/*36*< L31, Tempo ammollo a vapore fase 22 soft */
				15, 	/*37*< L32, Tempo erogazione detergente fase 24 standard taglia 6 */
				15, 	/*38*< L321, Tempo erogazione detergente fase 24 standard taglia 10 */
				10, 	/*39*< L33, Tempo erogazione detergente fase 24 soft taglia 6 */
				10, 	/*40*< L331, Tempo erogazione detergente fase 24 soft taglia 10 */
				360, 	/*41*< L34, Tempo reazione fase 25 standard */
				300, 	/*42*< L35, Tempo reazione fase 25 soft */
				240, 	/*43*< L36, Tempo reazione fase 27 standard */
				180, 	/*44*< L37, Tempo reazione fase 27 soft */
				60, 	/*45*< L38, Tempo risciacquo fase 28, 30 Standard */
				50, 	/*46*< L39, Tempo risciacquo fase 28, 30 Medium */
				40, 	/*47*< L40, Tempo risciacquo fase 28, 30 Basic */
				30, 	/*48*< L41, Tempo risciacquo fase 28, 30 Soft */
				20, 	/*49*< L42, Tempo risciacquo fase 28, 30 Decalcificazione */
				80, 	/*50*< L43, Tempo risciacquo fase 28, 30 Risciacquo */
				40, 	/*51*< L44, Tempo risciacquo fase 29 */
				40, 	/*52*< L45, Tempo attesa fase 31, 39 */
				20, 	/*53*< L46, Tempo erogazione brillantante fase 32, 34 standard taglia 6 */
				20, 	/*54*< L461, Tempo erogazione brillantante fase 32, 34 standard taglia 10 */
				15, 	/*55*< L47, Tempo erogazione brillantante fase 32, 34 medium taglia 6 */
				15, 	/*56*< L471, Tempo erogazione brillantante fase 32, 34 medium taglia 10 */
				10, 	/*57*< L48, Tempo erogazione brillantante fase 32, 34 basic taglia 6 */
				10, 	/*58*< L481, Tempo erogazione brillantante fase 32, 34 medium taglia 10 */
				8, 		/*59*< L49, Tempo erogazione brillantante fase 32, 34 soft taglia 6 */
				8, 		/*60*< L491, Tempo erogazione brillantante fase 32, 34 soft taglia 10 */
				25, 	/*61*< L50, Tempo erogazione brillantante fase 32, 34 decalcificazione taglia 6 */
				25, 	/*62*< L501, Tempo erogazione brillantante fase 32, 34 decalcificazione taglia 10 */
				210, 	/*63*< L51, Tempo reazione fase 33, 35 standard */
				180, 	/*64*< L52, Tempo reazione fase 33, 35 medium */
				150, 	/*65*< L53, Tempo reazione fase 33, 35 basic */
				120, 	/*66*< L54, Tempo reazione fase 33, 35 soft */
				240, 	/*67*< L55, Tempo reazione fase 33, 35 decalcificazione */
				180, 	/*68*< L56, Tempo risciacquo fase 36, 38 standard taglia 6 */
				180, 	/*69*< L561, Tempo risciacquo fase 36, 38 standard taglia 10 */
				150, 	/*70*< L57, Tempo risciacquo fase 36, 38 medium taglia 6 */
				150, 	/*71*< L571, Tempo risciacquo fase 36, 38 medium taglia 10 */
				120, 	/*72*< L58, Tempo risciacquo fase 36, 38 basic taglia 6 */
				120, 	/*73*< L581, Tempo risciacquo fase 36, 38 basic taglia 10 */
				90, 	/*74*< L59, Tempo risciacquo fase 36, 38 soft taglia 6 */
				90, 	/*75*< L591, Tempo risciacquo fase 36, 38 soft taglia 10 */
				150, 	/*76*< L60, Tempo risciacquo fase 36, 38 decalcificazione taglia 6 */
				150, 	/*77*< L601, Tempo risciacquo fase 36, 38 decalcificazione taglia 10 */
				180, 	/*78*< L61, Tempo risciacquo fase 37 standard taglia 6 */
				180, 	/*79*< L611, Tempo risciacquo fase 37 standard taglia 10 */
				150, 	/*80*< L62, Tempo risciacquo fase 37 medium taglia 6 */
				150, 	/*81*< L621, Tempo risciacquo fase 37 medium taglia 10 */
				120, 	/*82*< L63, Tempo risciacquo fase 37 basic taglia 6 */
				120, 	/*83*< L631, Tempo risciacquo fase 37 basic taglia 10 */
				90, 	/*84*< L64, Tempo risciacquo fase 37 soft taglia 6 */
				90, 	/*85*< L641, Tempo risciacquo fase 37 soft taglia 10 */
				150, 	/*86*< L65, Tempo risciacquo fase 37 decalcificazione taglia 6 */
				150, 	/*87*< L651, Tempo risciacquo fase 37 decalcificazione taglia 10 */
				240, 	/*88*< L66, Tempo asciugatura fase 40 standard */
				180, 	/*89*< L67, Tempo asciugatura fase 40 medium */
				180, 	/*90*< L68, Tempo asciugatura fase 40 basic */
				120, 	/*91*< L69, Tempo asciugatura fase 40 soft */
				356, 	/*92*< L70, Temperatura di asciugatura fase 40 */
				5, 		/*93*< L71, Erogazione brillantante in presenza di allarme */
				0, 		/*94*< L72, Correzione tempo di erogazione detergente */
				0 		/*95*< L73, Correzione tempo di erogazione brillantante */
		},
		//.AB={0,1},
		//.StartStopCnt=3,.AA[23]
		//.u16_crc=12
};

/*==================================================================
                       DEFINITION OF FUNCTIONS
==================================================================*/

static bool NVM_RestoreDefaults(void);
static uint16_t crc16Calculate( sNvmStorage_t dataStruct);

/*==================================================================
                       DECLARATION OF FUNCTIONS
==================================================================*/


void NVM_Init_Temporany(void)
{

	memcpy(&currNvm, &defaultNvm, sizeof(sNvmStorage_t));

	currNvm.AA[26]= *((uint32_t *)ADDR_FLASH_SECTOR_8);

	switch(currNvm.AA[26])
	{
	case 1:
		currOvenType=ov611;
		break;
	case 2:
		currOvenType=ov623;
		break;
	case 3:
		currOvenType=ov101;
		break;
	default:
		currOvenType=ovNOT_CONFIGURATED;
		break;
	}

}

bool saveOvenType(uint32_t dataOvenType)
{
	bool static ret=false;

	if (FLASH_OK == flash_erase(ADDR_FLASH_SECTOR_8,1))
	{
		ret=true;
	}
	else
	{
		ret=false;
	}
	if (FLASH_OK == flash_write(ADDR_FLASH_SECTOR_8, &dataOvenType, sizeof(dataOvenType)/4u ))
	{
		currNvm.AA[26]=dataOvenType;
		ret=true;
	}
	else
	{
		ret=false;
	}

	return ret;
}

/********************************************************************/
/** @brief Initialize NVM module configuration with one of the two
 * configuration areas already in memory, or in case of corrupted configuration
 * (checks with CRC) load the default configuration.
 *
 * @return @c true on success,@c false otherwise
 * */
/********************************************************************/
bool NVM_Init_struct(void)
{


	uint16_t crc16=0;
	uint16_t crc16read=0;

	memcpy(&currNvm, (void *) ADDR_FLASH_SECTOR_8, sizeof(sNvmStorage_t));
	crc16read=currNvm.u16_crc;
	crc16=crc16Calculate(currNvm);

	/* check CRC */
	if( crc16!=crc16read)
	{
		/*copy default in current*/
		NVM_RestoreDefaults();
	}

    /*recovery handling*/

	return false;
}

/********************************************************************/
/** @brief Store in memory the default configuration
 * @return @ true on success, @c false otherwise
 * */
/********************************************************************/
bool NVM_RestoreDefaults(void)
{
	bool static ret=false;
	bool static flashwrite=true;

	memcpy(&currNvm, &defaultNvm, sizeof(sNvmStorage_t));

	memcpy(&s_nvmData, &currNvm, sizeof(sNvmStorage_t));

	if (FLASH_OK == flash_erase(ADDR_FLASH_SECTOR_8,1))
	{
		ret=true;
	}
	else
	{
		ret=false;
	}

	if (FLASH_OK == flash_write(ADDR_FLASH_SECTOR_8, (uint32_t*)s_nvmData, sizeof(sNvmStorage_t) ))
	{
		/* Flashing error. */
		ret=true;
	}
	else
	{
		ret=false;
	}

	if(flashwrite)/*write in flash default config*/
	{
		/*copy default in curr*/
		memcpy(&currNvm, &defaultNvm, sizeof(sNvmStorage_t));
		ret = true;
	}
	else
	{
		ret=false;
	}

	return ret;

}

/********************************************************************/
/** @brief Store the actual configuration information inside the
 * two configuration areas of the memory
 * @return @ true if the configuration are store correctly in both areas,
 * @c false otherwise
 *
 * */
/********************************************************************/
bool NVM_SaveStorage(void)
{
    /* compute NVM crc */
    //((sNvmStorage_t *) s_nvmData)->u16_crc = crcFast((uint8_t *) s_nvmData, offsetof(sNvmStorage_t, u16_crc));

    /* Erase Image 1 */


    /* Write Image 1 */


    /* Check Image 1 */


    /* Erase Image 2 */


    /* Write Image 2 */


    /* Check Image 2 */
return false;
}

void set_NVM_Param(sParametersGroup groupParam, uint16_t index, uint16_t val)
{
	switch(groupParam)
	{
	case A_PARAMETERS:
		currNvm.A[index]=val;
		break;

	case AA_PARAMETERS:
		currNvm.AA[index]=val;
		break;

	case M_PARAMETERS:
		currNvm.M[index]=val;
		break;

	case V_PARAMETERS:
		currNvm.V[index]=val;
		break;

	case L_PARAMETERS:
		currNvm.L[index]=val;
		break;

	}
}

void return_NVM_Param(sParametersGroup groupParam, uint16_t index)
{
	switch(groupParam)
	{
	case A_PARAMETERS:
		usSRegInBuf[INDEX_PARAM]=index;
		usSRegInBuf[VALUE_PARAM]=currNvm.A[index];
		usSRegInBuf[PARAM_CNT]++;
		break;

	case AA_PARAMETERS:
		usSRegInBuf[INDEX_PARAM]=index;
		usSRegInBuf[VALUE_PARAM]=currNvm.AA[index];
		usSRegInBuf[PARAM_CNT]++;
		break;

	case M_PARAMETERS:
		usSRegInBuf[INDEX_PARAM]=index;
		usSRegInBuf[VALUE_PARAM]=currNvm.M[index];
		usSRegInBuf[PARAM_CNT]++;
		break;

	case V_PARAMETERS:
		usSRegInBuf[INDEX_PARAM]=index;
		usSRegInBuf[VALUE_PARAM]=currNvm.V[index];
		usSRegInBuf[PARAM_CNT]++;
		break;

	case L_PARAMETERS:
		usSRegInBuf[INDEX_PARAM]=index;
		usSRegInBuf[VALUE_PARAM]=currNvm.L[index];
		usSRegInBuf[PARAM_CNT]++;
		break;
	}
}

uint16_t crc16Calculate( sNvmStorage_t dataStruct)
{
	uint16_t crcResult =45;

	return crcResult;
}
