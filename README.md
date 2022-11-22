# Traccia di Laboratorio – Reti di calcolatori
***
# Progettare e implementare (scrivendo codice portabile) un’applicazione TCP client/server conforme al seguente protocollo:  
1. Il server è avviato su una data porta (NB: Il server resta sempre in ascolto).
2. Il client legge da tastiera il numero di porta da utilizzare per contattare il Server
3. Il client, richiede la connessione al server.
4. Stabilita la connessione, il server visualizza sullo std output un messaggio contenente il l’ip del client con cui ha stabilito la connessione. (Esempio: “connessione stabilita con il client: XYZ”)
5. Il server invia al client la stringa "connessione avvenuta".
6. Il client legge una dallo std input due stringhe A e B e le invia al server.
7. Il server riceve le stringhe A e B e le concatena in un'unica stringa C uguale ad “A + B” e la invia indietro al client; altrimenti, se almeno una delle due stringhe A o B è uguale a “quit” il server invia al client la stringa “bye”.
8. Il client riceve la stringa inviata dal server e la stampa a video. Se la stringa visualizzata è uguale a “bye”, chiude la connessione e il processo termina; altrimenti, il client torna al passo 6.