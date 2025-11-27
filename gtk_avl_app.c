// gcc gtk_avl_app.c -o app `pkg-config --cflags --libs gtk+-3.0`

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 //Arvore AVL
typedef struct no {
    int valor;
    struct no *esquerda, *direita;
} NoArv;

NoArv *raiz = NULL;
int vetor[100];
int cnt = 0;

NoArv* inserir_versao_01(NoArv *raiz, int num){
    if(raiz == NULL){
        NoArv *aux = malloc(sizeof(NoArv));
        aux->valor = num;
        aux->esquerda = NULL;
        aux->direita = NULL;
        return aux;
    } else {
        if(num < raiz->valor)
            raiz->esquerda = inserir_versao_01(raiz->esquerda, num);
        else
            raiz->direita = inserir_versao_01(raiz->direita, num);
        return raiz;
    }
}

int calcular_altura(NoArv *raiz){
    if(raiz == NULL)
        return -1;
    int esq = calcular_altura(raiz->esquerda);
    int dir = calcular_altura(raiz->direita);
    return (esq > dir) ? esq + 1 : dir + 1;
}

void listar_pre_ordem(NoArv *raiz, char *buffer){
    if(raiz){
        char temp[50];
        sprintf(temp, "%d ", raiz->valor);
        strcat(buffer, temp);
        listar_pre_ordem(raiz->esquerda, buffer);
        listar_pre_ordem(raiz->direita, buffer);
    }
}

void ordenar_decrescente(int vetor[], int cnt){
    int i, j, temp;
    for(i=0;i<cnt-1;i++){
        for(j=i+1;j<cnt;j++){
            if(vetor[i] < vetor[j]){
                temp = vetor[i];
                vetor[i] = vetor[j];
                vetor[j] = temp;
            }
        }
    }
}


GtkWidget *entry_valor; //objeto de entrada de texto
GtkWidget *txt_output; //leitura do texto

void escrever_output(const char *texto){
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(txt_output));
    gtk_text_buffer_set_text(buffer, texto, -1);
}


void on_btn_inserir(GtkButton *b, gpointer data){
    const char *txt = gtk_entry_get_text(GTK_ENTRY(entry_valor));
    int valor = atoi(txt);

    vetor[cnt++] = valor;
    raiz = inserir_versao_01(raiz, valor);

    escrever_output("Valor inserido!");
    gtk_entry_set_text(GTK_ENTRY(entry_valor), "");
}

void on_btn_imprimir(GtkButton *b, gpointer data){
    char buffer[2048] = "";
    listar_pre_ordem(raiz, buffer);
    escrever_output(buffer);
}

void on_btn_altura(GtkButton *b, gpointer data){
    char buffer[50];
    sprintf(buffer, "Altura da árvore: %d", calcular_altura(raiz));
    escrever_output(buffer);
}

void on_btn_decrescente(GtkButton *b, gpointer data){
    ordenar_decrescente(vetor, cnt);

    char buffer[2048] = "";
    char temp[50];
    for(int i=0;i<cnt;i++){
        sprintf(temp, "%d ", vetor[i]);
        strcat(buffer, temp);
    }

    escrever_output(buffer);
}


int main(int argc, char *argv[]){
    gtk_init(&argc, &argv);

    GtkWidget *janela = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(janela), "Árvore AVL - GTK");
    gtk_window_set_default_size(GTK_WINDOW(janela), 400, 400);
    g_signal_connect(janela, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(janela), vbox);

    // Entrada + botão inserir
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    entry_valor = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_valor), "Digite um número");

    GtkWidget *btn_inserir = gtk_button_new_with_label("Inserir");
    g_signal_connect(btn_inserir, "clicked", G_CALLBACK(on_btn_inserir), NULL);

    gtk_box_pack_start(GTK_BOX(hbox), entry_valor, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), btn_inserir, FALSE, FALSE, 0);

    // Botões extras
    GtkWidget *btn_imprimir = gtk_button_new_with_label("Imprimir pré-ordem");
    GtkWidget *btn_altura = gtk_button_new_with_label("Altura");
    GtkWidget *btn_decrescente = gtk_button_new_with_label("Decrescente");

    g_signal_connect(btn_imprimir, "clicked", G_CALLBACK(on_btn_imprimir), NULL);
    g_signal_connect(btn_altura, "clicked", G_CALLBACK(on_btn_altura), NULL);
    g_signal_connect(btn_decrescente, "clicked", G_CALLBACK(on_btn_decrescente), NULL);

    gtk_box_pack_start(GTK_BOX(vbox), btn_imprimir, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), btn_altura, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), btn_decrescente, FALSE, FALSE, 0);

    // Área de texto
    txt_output = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(txt_output), FALSE);

    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scroll), txt_output);
    gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 0);

    gtk_widget_show_all(janela);
    gtk_main();

    return 0;
}
