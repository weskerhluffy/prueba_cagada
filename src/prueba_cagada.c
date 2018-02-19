/*
 * prueba_cagada.c
 *
 *  Created on: 18/02/2018
 *      Author: ernesto
 */

// XXX: https://uva.onlinejudge.org/index.php?option=onlinejudge&page=show_problem&problem=1672
#if 1
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <stdint.h>

#ifdef CACA_COMUN_LOG
#include <execinfo.h>
#endif

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#else
#include <time.h>
#include <stdarg.h>
#endif

#if 1
#ifndef LONG_LONG_MAX
#define LONG_LONG_MAX LONG_MAX
#endif

#define CACA_COMUN_TAM_MAX_LINEA (16*200000)
#define CACA_LOG_MAX_TAM_CADENA 2000

#define CACA_COMUN_BUF_STATICO (char[1000] ) { '\0' }

#define BITCH_VECTOR_NUM_BITS (sizeof(bitch_vector) * 8)

#define CACA_COMUN_ASSERT_DUROTE 0
#define CACA_COMUN_ASSERT_SUAVECITO 1
#define CACA_COMUN_ASSERT_NIMADRES 2

#define CACA_COMUN_VALOR_INVALIDO ((tipo_dato)UINT_MAX)
#define CACA_COMUN_IDX_INVALIDO ((natural)CACA_COMUN_VALOR_INVALIDO)

typedef unsigned int natural;
typedef natural tipo_dato;
typedef long long entero_largo;
typedef unsigned long long entero_largo_sin_signo;
typedef long long bitch_vector;
typedef char byteme;

typedef enum BOOLEANOS {
	falso = 0, verdadero
} bool;

/*
 #define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_SUAVECITO
 #define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_NIMADRES
 */
#define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_DUROTE

#define assert_timeout_dummy(condition) 0;

#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_DUROTE
#define assert_timeout(condition) assert(condition);
#endif
#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_SUAVECITO
#define assert_timeout(condition) if(!(condition)){while(1){printf("");};abort();}
#endif
#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_NIMADRES
#define assert_timeout(condition) 0
#endif

#ifdef CACA_COMUN_LOG
#define caca_log_debug(formato, args...) \
		do \
		{ \
			size_t profundidad = 0; \
			void *array[CACA_LOG_MAX_TAM_CADENA]; \
 			profundidad = backtrace(array, CACA_LOG_MAX_TAM_CADENA); \
			caca_log_debug_func(formato,__FILE__, __func__, __LINE__,profundidad,##args); \
		} \
		while(0);
#else
#define caca_log_debug(formato, args...) 0;
#endif

#define caca_comun_max(x,y) ((x) < (y) ? (y) : (x))
#define caca_comun_min(x,y) ((x) < (y) ? (x) : (y))

void caca_log_debug_func(const char *format, ...);

void caca_comun_current_utc_time(struct timespec *ts) {

#ifdef __MACH__
	clock_serv_t cclock;
	mach_timespec_t mts;
	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	clock_get_time(cclock, &mts);
	mach_port_deallocate(mach_task_self(), cclock);
	ts->tv_sec = mts.tv_sec;
	ts->tv_nsec = mts.tv_nsec;
#else
#ifdef CACA_COMUN_LOG
	clock_gettime(CLOCK_REALTIME, ts);
#endif
#endif

}

void caca_comun_timestamp(char *stime) {
	time_t ltime;
	struct tm result;
	long ms;
#ifndef ONLINE_JUDGE
	struct timespec spec;
#endif
	char parte_milisecundos[50];

	ltime = time(NULL);

#ifndef ONLINE_JUDGE
	localtime_r(&ltime, &result);
	asctime_r(&result, stime);
#endif

	*(stime + strlen(stime) - 1) = ' ';

#ifndef ONLINE_JUDGE
	caca_comun_current_utc_time(&spec);
	ms = round(spec.tv_nsec / 1.0e3);
#endif
	sprintf(parte_milisecundos, "%ld", ms);
	strcat(stime, parte_milisecundos);
}
#ifdef CACA_COMUN_LOG
void caca_log_debug_func(const char *format, ...) {

	va_list arg;
	va_list arg2;
	const char *PEDAZO_TIMESTAMP_HEADER = "tiempo: %s; ";
	const char *HEADER =
	"archivo: %s; funcion: %s; linea %d; nivel: %zd caca 8====D ";
	char formato[CACA_LOG_MAX_TAM_CADENA + sizeof(HEADER)
	+ sizeof(PEDAZO_TIMESTAMP_HEADER)] = {'\0'};
	char pedazo_timestamp[sizeof(PEDAZO_TIMESTAMP_HEADER) + 100] = {'\0'};
	char cadena_timestamp[100] = {'\0'};

	caca_comun_timestamp(cadena_timestamp);
	sprintf(pedazo_timestamp, PEDAZO_TIMESTAMP_HEADER, cadena_timestamp);

	strcpy(formato, pedazo_timestamp);
	strcat(formato, HEADER);
	strcat(formato, format);
	strcat(formato, "\n");

	va_start(arg, format);
	va_copy(arg2, arg);
	vprintf(formato, arg2);
	va_end(arg2);
	va_end(arg);
	setbuf(stdout, NULL);
}
#endif

#ifdef CACA_COMUN_LOG
static char *caca_comun_arreglo_a_cadena(tipo_dato *arreglo, int tam_arreglo,
		char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos,
				"%12d", *(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}

static char *caca_comun_arreglo_a_cadena_natural(natural *arreglo,
		natural tam_arreglo, char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos, "%2u",
				*(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}
char *caca_comun_matrix_a_cadena(tipo_dato *matrix, natural filas_tam,
		natural columas_tam, char *buffer) {
	int i, j;
	natural inicio_buffer_act = 0;
	for (int i = 0; i < filas_tam; i++) {
		caca_comun_arreglo_a_cadena(matrix + i * columas_tam, columas_tam,
				buffer + inicio_buffer_act);
		inicio_buffer_act += strlen(buffer + inicio_buffer_act);
		buffer[inicio_buffer_act++] = '\n';
		/*		caca_log_debug("pero q mierda inicio buffer act %u %s",inicio_buffer_act,buffer);*/
	}
	return buffer;
}

static char *caca_comun_arreglo_a_cadena_char(char *arreglo,
		natural tam_arreglo, char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos, "%2u",
				*(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}

char *caca_comun_matrix_a_cadena_char(char *matrix, natural filas_tam,
		natural columas_tam, char *buffer) {
	int i, j;
	natural inicio_buffer_act = 0;
	for (int i = 0; i < filas_tam; i++) {
		caca_comun_arreglo_a_cadena_char(matrix + i * columas_tam, columas_tam,
				buffer + inicio_buffer_act);
		inicio_buffer_act += strlen(buffer + inicio_buffer_act);
		buffer[inicio_buffer_act++] = '\n';
		/*		caca_log_debug("pero q mierda inicio buffer act %u %s",inicio_buffer_act,buffer);*/
	}
	return buffer;
}

static char *caca_comun_arreglo_a_cadena_entero_largo_sin_signo(entero_largo_sin_signo *arreglo,
		entero_largo_sin_signo tam_arreglo, char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos, "%2llu",
				*(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}
#else
static char *caca_comun_arreglo_a_cadena(tipo_dato *arreglo, int tam_arreglo,
		char *buffer) {
	return NULL;
}
static char *caca_comun_arreglo_a_cadena_natural(natural *arreglo,
		natural tam_arreglo, char *buffer) {
	return NULL;
}
static char *caca_comun_arreglo_a_cadena_char(char *arreglo,
		natural tam_arreglo, char *buffer);

char *caca_comun_matrix_a_cadena_char(char *matrix, natural filas_tam,
		natural columas_tam, char *buffer);
char *caca_comun_matrix_a_cadena(tipo_dato *matrix, natural filas_tam,
		natural columas_tam, char *buffer) {
	return NULL;
}

static char *caca_comun_arreglo_a_cadena_entero_largo_sin_signo(
		entero_largo_sin_signo *arreglo, entero_largo_sin_signo tam_arreglo,
		char *buffer) {
	return NULL;
}
#endif
void caca_comun_strreplace(char s[], char chr, char repl_chr) {
	int i = 0;
	while (s[i] != '\0') {
		if (s[i] == chr) {
			s[i] = repl_chr;
		}
		i++;
	}
}

static int caca_comun_lee_matrix_long_stdin(tipo_dato *matrix, int *num_filas,
		int *num_columnas, int num_max_filas, int num_max_columnas) {
	int indice_filas = 0;
	int indice_columnas = 0;
	tipo_dato numero = 0;
	char *siguiente_cadena_numero = NULL;
	char *cadena_numero_actual = NULL;
	char *linea = NULL;

	linea = calloc(CACA_COMUN_TAM_MAX_LINEA, sizeof(char));

	while (indice_filas < num_max_filas
			&& fgets(linea, CACA_COMUN_TAM_MAX_LINEA, stdin)) {
		indice_columnas = 0;
		cadena_numero_actual = linea;
		caca_comun_strreplace(linea, '\n', '\0');
		if (!strlen(linea)) {
			caca_log_debug("weird, linea vacia");
			continue;
		}
		for (siguiente_cadena_numero = linea;; siguiente_cadena_numero =
				cadena_numero_actual) {
			numero = strtol(siguiente_cadena_numero, &cadena_numero_actual, 10);
			if (cadena_numero_actual == siguiente_cadena_numero) {
				break;
			}
			*(matrix + indice_filas * num_max_columnas + indice_columnas) =
					numero;
			indice_columnas++;
		}
		if (num_columnas) {
			num_columnas[indice_filas] = indice_columnas;
		}
		indice_filas++;
		caca_log_debug("las filas son %d, con clos %d", indice_filas,
				indice_columnas);
	}

	*num_filas = indice_filas;
	free(linea);
	return 0;
}

static inline natural caca_comun_cuenta_bitchs(tipo_dato num) {
	natural bitch_cnt = 0;
	tipo_dato num_mod = 0;
	num_mod = num;
	while (num_mod) {
		num_mod &= ~(num_mod & (-num_mod));
		bitch_cnt++;
	}
	return bitch_cnt;
}

static inline char caca_comun_letra_a_valor_minuscula(char letra) {
	return letra - 'a';
}

static inline char caca_comun_letra_a_valor_mayuscula(char letra) {
	return letra - 'A';
}

static inline natural caca_comun_max_natural(natural *nums, natural nums_tam) {
	natural max = 0;

	for (int i = 0; i < nums_tam; i++) {
		natural num_act = nums[i];
		if (num_act > max) {
			max = num_act;
		}
	}

	return max;
}

#endif
#endif

#if 1

// XXX: https://lispmachine.wordpress.com/2009/05/13/queue-in-c/
typedef struct my_struct {
	void *valor;
	struct my_struct* next;
} listilla_nodo;

typedef struct my_list {
	struct my_struct* head;
	struct my_struct* tail;
	natural elementos_cnt;
} listilla_fifo;

typedef struct listilla_iterador {
	listilla_fifo *ctx;
	listilla_nodo *nodo_act;
	bool primera_llamada;
	natural llamadas;
} listilla_iterador;

/* Will always return the pointer to my_list */
struct my_list* list_add_element(struct my_list* s, void *valor) {
	struct my_struct* p = malloc(1 * sizeof(*p));

	if ( NULL == p) {
		fprintf(stderr, "IN %s, %s: malloc() failed\n", __FILE__, "list_add");
		return s;
	}

	p->valor = valor;
	p->next = NULL;

	if ( NULL == s) {
		printf("Queue not initialized\n");
		free(p);
		return s;
	} else if ( NULL == s->head && NULL == s->tail) {
		/* printf("Empty list, adding p->num: %d\n\n", p->num);  */
		s->head = s->tail = p;
		return s;
	} else if ( NULL == s->head || NULL == s->tail) {
		fprintf(stderr,
				"There is something seriously wrong with your assignment of head/tail to the list\n");
		free(p);
		return NULL;
	} else {
		/* printf("List not empty, adding element to tail\n"); */
		s->tail->next = p;
		s->tail = p;
	}
	s->elementos_cnt++;

	return s;
}

/* This is a queue and it is FIFO, so we will always remove the first element */
static inline void *list_remove_element(struct my_list* s) {
	struct my_struct* h = NULL;
	struct my_struct* p = NULL;
	void *valor = NULL;

	if ( NULL == s) {
		printf("List is empty\n");
		return s;
	} else if ( NULL == s->head && NULL == s->tail) {
		printf("Well, List is empty\n");
		return s;
	} else if ( NULL == s->head || NULL == s->tail) {
		printf("There is something seriously wrong with your list\n");
		printf("One of the head/tail is empty while other is not \n");
		abort();
	}

	h = s->head;
	valor = h->valor;
	p = h->next;
	free(h);
	s->head = p;
	if ( NULL == s->head)
		s->tail = s->head; /* The element tail was pointing to is free(), so we need an update */

	s->elementos_cnt--;
	return valor;
}

/* ---------------------- small helper fucntions ---------------------------------- */
struct my_list* list_free(struct my_list* s) {
	while (s->head) {
		list_remove_element(s);
	}

	return s;
}

struct my_list* list_new(void) {
	struct my_list* p = malloc(1 * sizeof(*p));

	assert_timeout(p);

	p->head = p->tail = NULL;

	return p;
}

void list_print_element(const struct my_struct* p) {
	if (p) {
		printf("Num = %p\n", p->valor);
	} else {
		printf("Can not print NULL struct \n");
	}
}

void list_print(const struct my_list* ps) {
	struct my_struct* p = NULL;

	if (ps) {
		for (p = ps->head; p; p = p->next) {
			list_print_element(p);
		}
	}

	printf("------------------\n");
}

static inline bool list_empty(struct my_list *s) {
	return !s->head;
}

static inline void list_iterador_init(listilla_fifo *ctx,
		listilla_iterador *iter) {
	assert_timeout(!iter->ctx);
	assert_timeout(!iter->primera_llamada);
	assert_timeout(!iter->nodo_act);
	assert_timeout(!iter->llamadas);
	iter->ctx = ctx;
	iter->nodo_act = NULL;
	iter->primera_llamada = verdadero;
	iter->llamadas = 0;
}
static inline void list_iterador_fini(listilla_iterador *iter) {
	iter->ctx = NULL;
	iter->nodo_act = NULL;
	iter->primera_llamada = falso;
	iter->llamadas = 0;
}

static inline void *list_iterador_peekea_actual(listilla_iterador *iter) {
	return iter->nodo_act ? iter->nodo_act->valor : NULL;
}

static inline void *list_iterador_obten_siguiente(listilla_iterador *iter) {
	if (iter->nodo_act) {
		iter->nodo_act = iter->nodo_act->next;
	} else {
		if (iter->primera_llamada) {
			iter->primera_llamada = falso;
			iter->nodo_act = iter->ctx->head;
		}
	}
	if (iter->nodo_act) {
		iter->llamadas++;
	}
	return iter->nodo_act ? iter->nodo_act->valor : NULL;
}
static inline void *list_iterador_hay_siguiente(listilla_iterador *iter) {
	listilla_nodo *siguiente = NULL;
	if (!iter->nodo_act) {
		if (iter->primera_llamada) {
			siguiente = iter->ctx->head;
		}
	} else {
		siguiente = iter->nodo_act->next;
	}

	return siguiente ? siguiente->valor : NULL;
}

static inline bool list_iterador_esta_initializado(listilla_iterador *iter) {
	return !!iter->ctx;
}

#endif

#if 1
// XXX: http://www.zentut.com/c-tutorial/c-stack-using-pointers/
typedef struct stackaca_nodo {
	void *data;
	struct stackaca_nodo* next;
} stackaca_nodo;
typedef struct stackaca {
	stackaca_nodo *cabeza;
} stackaca;

/*
 init the stack
 */
static inline void stackaca_init(stackaca *ctx) {
	ctx->cabeza = NULL;
}

/*
 push an element into stack
 */
static inline void stackaca_push(stackaca *ctx, void *data) {
	stackaca_nodo* tmp = (stackaca_nodo*) malloc(sizeof(stackaca_nodo));
	assert_timeout(tmp);
	tmp->data = data;
	tmp->next = ctx->cabeza;
	ctx->cabeza = tmp;
}
/*
 pop an element from the stack
 */
static inline void *stackaca_pop(stackaca *ctx) {
	stackaca_nodo* tmp = ctx->cabeza;
	void *caca = tmp->data;
	ctx->cabeza = tmp->next;
	free(tmp);
	return caca;
}
/*
 returns 1 if the stack is empty, otherwise returns 0
 */
static inline bool stackaca_empty(stackaca *ctx) {
	return ctx->cabeza == NULL ? verdadero : falso;
}

static inline void stackaca_fini(stackaca *ctx) {
	while (!stackaca_empty(ctx)) {
		stackaca_pop(ctx);
	}
}

static inline void *stackaca_peekea(stackaca *ctx) {
	return ctx->cabeza ? ctx->cabeza->data : NULL;
}
#endif

#define PRUEBA_CAGADA_PRIMERA_LETRA 'A'
#define PRUEBA_CAGADA_ULTIMA_LETRA 'Z'
#define PRUEBA_CAGADA_FIN_DE_CACADENA '\0'
#define PRUEBA_CAGADA_MAX_LETRAS (PRUEBA_CAGADA_ULTIMA_LETRA-PRUEBA_CAGADA_PRIMERA_LETRA+1)
#define PRUEBA_CAGADA_INDICE_INVALIDO -1
typedef struct prueba_cagada_nodo {
	char letra;
	bool en_culada;
	natural indice;
	natural enlace_bajo;
	listilla_fifo *vecinos;
	listilla_iterador iter;
} prueba_cagada_nodo;

char matrix_componentes[PRUEBA_CAGADA_MAX_LETRAS][PRUEBA_CAGADA_MAX_LETRAS] = {
PRUEBA_CAGADA_FIN_DE_CACADENA };

// XXX: https://stackoverflow.com/questions/1106957/passing-an-array-by-reference-in-c
static inline void prueba_cagada_tarja_dfs(prueba_cagada_nodo *nodo,
		char (*matrix_componentes_p)[PRUEBA_CAGADA_MAX_LETRAS][PRUEBA_CAGADA_MAX_LETRAS],
		natural *indice) {
	stackaca *pila_recursividad = &(stackaca ) { 0 };
	stackaca *pila_de_tarja = &(stackaca ) { 0 };

	stackaca_init(pila_recursividad);
	stackaca_init(pila_de_tarja);

	stackaca_push(pila_recursividad, nodo);
	list_iterador_init(nodo->vecinos, &nodo->iter);

	while (!stackaca_empty(pila_recursividad)) {
		prueba_cagada_nodo *nodo_act = stackaca_pop(pila_recursividad);
		assert_timeout(nodo_act->letra);

		caca_log_debug("en nodo %c con (%d,%d)", nodo_act->letra,
				nodo_act->indice, nodo_act->enlace_bajo);

		if (nodo_act->indice == PRUEBA_CAGADA_INDICE_INVALIDO) {
			nodo_act->enlace_bajo = nodo_act->indice = (*indice)++;
			caca_log_debug("aora nodo %c es (%d,%d)", nodo_act->letra,
					nodo_act->indice, nodo_act->enlace_bajo);
			stackaca_push(pila_de_tarja, nodo_act);
			nodo_act->en_culada = 1;
			if (!list_iterador_esta_initializado(&nodo_act->iter)) {
				list_iterador_init(nodo_act->vecinos, &nodo_act->iter);
			}
		}

		if (list_iterador_hay_siguiente(&nodo_act->iter)) {
			prueba_cagada_nodo *vecino = NULL;

			vecino = list_iterador_obten_siguiente(&nodo_act->iter);
			caca_log_debug("checando vecino %c", vecino->letra);

			stackaca_push(pila_recursividad, nodo_act);

			if (vecino->indice == PRUEBA_CAGADA_INDICE_INVALIDO) {
				caca_log_debug("se enculo %c", vecino->letra);
				stackaca_push(pila_recursividad, vecino);
			}

		} else {
			listilla_iterador *iter = &(listilla_iterador ) { 0 };

			list_iterador_init(nodo_act->vecinos, iter);
			caca_log_debug("nodo %c ia visito todo", nodo_act->letra);

			while (list_iterador_hay_siguiente(iter)) {
				prueba_cagada_nodo *vecino = list_iterador_obten_siguiente(
						iter);
				caca_log_debug("revisando vecino %c (%d,%d)", vecino->letra,
						vecino->indice, vecino->enlace_bajo);
				if (vecino->indice > nodo_act->indice) {
					caca_log_debug("comparando %d con %d",
							nodo_act->enlace_bajo, vecino->enlace_bajo);
					nodo_act->enlace_bajo = caca_comun_min(
							nodo_act->enlace_bajo, vecino->enlace_bajo);
				} else {
					if (vecino->en_culada) {
						nodo_act->enlace_bajo = caca_comun_min(
								nodo_act->enlace_bajo, vecino->indice);
						caca_log_debug("en ciclo aora enlace bajo es %d",
								nodo_act->enlace_bajo);
					}
					caca_log_debug("en ijo aora enlace bajo es %d",
							nodo_act->enlace_bajo);
				}
			}

			if (nodo_act->indice == nodo_act->enlace_bajo) {
				prueba_cagada_nodo *nodo_componente = NULL;
				do {
					nodo_componente = stackaca_pop(pila_de_tarja);
					caca_log_debug("en gpo %c esta %c", nodo_act->letra,
							nodo_componente->letra);
					assert_timeout(nodo_componente->en_culada);
					assert_timeout(
							nodo_componente->indice!=PRUEBA_CAGADA_INDICE_INVALIDO);
					nodo_componente->en_culada = falso;
					(*matrix_componentes_p)[caca_comun_letra_a_valor_mayuscula(
							nodo_act->letra)][caca_comun_letra_a_valor_mayuscula(
							nodo_componente->letra)] = 1;
				} while (nodo_componente != nodo_act);
				caca_log_debug("la matrix aora es\n%s",
						caca_comun_matrix_a_cadena_char((char*)*matrix_componentes_p, PRUEBA_CAGADA_MAX_LETRAS, PRUEBA_CAGADA_MAX_LETRAS, CACA_COMUN_BUF_STATICO));
			}
			list_iterador_fini(iter);
			list_iterador_fini(&nodo_act->iter);
		}

	}

	stackaca_fini(pila_recursividad);
	stackaca_fini(pila_de_tarja);
}

static inline void prueba_cagada_tarja(prueba_cagada_nodo *nodos,
		natural nodos_tam,
		char (*matrix_componentes_p)[PRUEBA_CAGADA_MAX_LETRAS][PRUEBA_CAGADA_MAX_LETRAS]) {

	natural indice = 0;
	for (natural i = 0; i < nodos_tam; i++) {
		prueba_cagada_nodo *nodo_act = nodos + i;
		if (nodo_act->indice == PRUEBA_CAGADA_INDICE_INVALIDO
				&& nodo_act->letra) {
			prueba_cagada_tarja_dfs(nodo_act, matrix_componentes_p, &indice);
		}
	}
}

static inline void prueba_cagada_core(prueba_cagada_nodo *nodos,
		natural nodos_tam,
		char (*matrix_componentes_p)[PRUEBA_CAGADA_MAX_LETRAS][PRUEBA_CAGADA_MAX_LETRAS]) {
	prueba_cagada_tarja(nodos, nodos_tam, matrix_componentes_p);

	caca_log_debug("pero q mierdas\n%s",
			caca_comun_matrix_a_cadena_char((char *)*matrix_componentes_p, PRUEBA_CAGADA_MAX_LETRAS, PRUEBA_CAGADA_MAX_LETRAS, CACA_COMUN_BUF_STATICO));

	for (char chx = PRUEBA_CAGADA_PRIMERA_LETRA, cx =
			caca_comun_letra_a_valor_mayuscula(chx);
			chx <= PRUEBA_CAGADA_ULTIMA_LETRA; chx++, cx =
					caca_comun_letra_a_valor_mayuscula(chx)) {
		bool hubo_caca = falso;
		for (char chy = PRUEBA_CAGADA_PRIMERA_LETRA, cy =
				caca_comun_letra_a_valor_mayuscula(chy);
				chy <= PRUEBA_CAGADA_ULTIMA_LETRA; chy++, cy =
						caca_comun_letra_a_valor_mayuscula(chy)) {
			caca_log_debug("puta madre %c %c %u %u", chx, chy, cx, cy);
			if ((*matrix_componentes_p)[cx][cy]) {
				caca_log_debug("gpo %c letra %c", chx, chy);
				if (hubo_caca) {
					printf(" ");
				}
				printf("%c", chy);
				hubo_caca = verdadero;
			}
		}
		if (hubo_caca) {
			printf("\n");
		}
	}
}

static inline void prueba_cagada_main() {
	natural conjuntos_tam = 0;
	while (scanf("%u\n", &conjuntos_tam) > 0 && conjuntos_tam > 0) {
		prueba_cagada_nodo *nodos = NULL;

		nodos = calloc(PRUEBA_CAGADA_MAX_LETRAS, sizeof(prueba_cagada_nodo));
		assert_timeout(nodos);

		for (natural i = 0; i < conjuntos_tam; i++) {
			char opciones[5] = { PRUEBA_CAGADA_FIN_DE_CACADENA };
			char respuesta = PRUEBA_CAGADA_FIN_DE_CACADENA;
			prueba_cagada_nodo *nodo_sup = NULL;
			prueba_cagada_nodo *nodo_act = NULL;

			scanf("%c %c %c %c %c %c\n", opciones, opciones + 1, opciones + 2,
					opciones + 3, opciones + 4, &respuesta);

			caca_log_debug("opciones %s elegida %c",
					caca_comun_arreglo_a_cadena_char(opciones, 5, CACA_COMUN_BUF_STATICO),
					respuesta);
			nodo_sup = nodos + caca_comun_letra_a_valor_mayuscula(respuesta);
			if (!nodo_sup->letra) {
				nodo_sup->letra = respuesta;
				nodo_sup->enlace_bajo = nodo_sup->indice =
				PRUEBA_CAGADA_INDICE_INVALIDO;
				nodo_sup->vecinos = list_new();
			}
			for (natural j = 0; j < 5; j++) {
				char letra_act = opciones[j];
				if (letra_act == respuesta) {
					continue;
				}
				nodo_act = nodos
						+ caca_comun_letra_a_valor_mayuscula(letra_act);

				if (!nodo_act->letra) {
					nodo_act->letra = letra_act;
					nodo_act->enlace_bajo = nodo_act->indice =
					PRUEBA_CAGADA_INDICE_INVALIDO;
					nodo_act->vecinos = list_new();
				}

				caca_log_debug("%c es padre de %c", respuesta, nodo_act->letra);

				list_add_element(nodo_sup->vecinos, nodo_act);
			}
		}

		memset(matrix_componentes, 0, sizeof(matrix_componentes));
		prueba_cagada_core(nodos, PRUEBA_CAGADA_MAX_LETRAS,
				&matrix_componentes);

		free(nodos);

		printf("\n");
	}
}

int main(void) {
	prueba_cagada_main();
	return EXIT_SUCCESS;
}
