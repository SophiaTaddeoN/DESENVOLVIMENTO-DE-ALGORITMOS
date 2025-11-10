import json
import time
import os


def limpar_tela():
    os.system('cls' if os.name == 'nt' else 'clear')

def cadastro():  

    arquivo_dados=open("dados.txt", "r")
    dados=json.load(arquivo_dados)
    arquivo_dados.close()
    print("=== CADASTRO ===")
    nome = input("Nome: ")
    user = input("User: ")
    senha = input("Senha: ")

    for usuario in dados["usuarios"]:
        if usuario["user"] == user:
            print("Usuário {} já cadastrado!".format(user))
            time.sleep(2)
            limpar_tela()
            
            print("=== CADASTRO ===")
            nvo_cadastro = input("Deseja tentar outro usuário? (S/N) ").upper()
            if nvo_cadastro == "S":
                
                return cadastro()
           
            else:
                return False

    novo_cadastro = {"nome": nome, "user": user, "senha": senha}
    dados["usuarios"].append(novo_cadastro)
    
    arquivo_ler=open("dados.txt","w")
    json.dump(dados, arquivo_ler,indent=4, ensure_ascii=False)
    arquivo_ler.close()
        
    print("Usuário cadastrado com sucesso!")
    time.sleep(2)
    limpar_tela()
    
    return True


def login():

    arquivo_dados=open("dados.txt", "r")
    dados=json.load(arquivo_dados)
    arquivo_dados.close()

    print("=== LOGIN ===")
    user = input("User: ")
    senha = input("Senha: ")
    for usuario in dados["usuarios"]:
        if usuario["user"] == user and usuario["senha"] == senha:
            
            print("Login realizado com sucesso! ")
            usuario_l=user
            time.sleep(2)
            limpar_tela()
            
            return usuario_l
    
    print("Usuário ou senha incorretos")
    time.sleep(2)
    limpar_tela()
    arquivo_dados=open("dados.txt", "r")
    dados=json.load(arquivo_dados)
    arquivo_dados.close()
    
    
    return False


def vizualizar_hist(user):

    arquivo_dados=open("dados.txt", "r")
    dados=json.load(arquivo_dados)
    arquivo_dados.close()

    print("=== VIZUALIZAR HISTÓRICO ===")
    print("\n1. Músicas Curtidas\n2. Músicas Não-Curtidas")

    ver_hist = input("\nOpção: ")

    time.sleep(1)
    limpar_tela()
    

    if ver_hist == "1":
        print("=== MÚSICAS CURTIDAS ===")
        for curtida in dados["historico"]["curtidas"]:
            if curtida["usuario"] == user:
                print("{} - {} ({})\n".format(curtida["musicas"]["musica"], curtida["musicas"]["artista"], curtida["musicas"]["duracao"]))
                time.sleep(6)
                limpar_tela()

            
                
    elif ver_hist == "2":
        print("=== MÚSICAS NÃO CURTIDAS ===")
        for n_curtida in dados["historico"]["nao_curtidas"]:
            if n_curtida["usuario"] == user:
                print("{} - {} ({})\n".format(n_curtida["musicas"]["musica"],n_curtida["musicas"]["artista"], n_curtida["musicas"]["duracao"]))
                time.sleep(6)
                limpar_tela()
                
def curtir_m(user,nome_musica):
    arquivo_dados=open("dados.txt", "r")
    dados=json.load(arquivo_dados)
    arquivo_dados.close()

    print("=== CURTIR MÚSICA ===\n")

    print("=== MÚSICAS JÁ CURTIDAS ===\n")

    for curtida in dados["historico"]["curtidas"]:
            if curtida["usuario"] == user:
                print("{} - {} ({})\n".format(curtida["musicas"]["musica"], curtida["musicas"]["artista"], curtida["musicas"]["duracao"]))
                break

    musica_curtida=[]
    for musica in dados["musicas"]:
        if musica["nome"].upper() == nome_musica:
            time.sleep(1)
            print("Música encontrada!")
            time.sleep(1)
            print("{} - {} - ({})".format(musica["nome"], musica["artista"], musica["duracao"]))
            musica_curtida = {
                "musica": musica["nome"],
                "artista": musica["artista"],
                "duracao": musica["duracao"]
                }
    if musica_curtida:
        dados["historico"]["curtidas"].append({
            "usuario": user,
            "musicas": musica_curtida,
            })
        print("Música curtida com sucesso!")
        time.sleep(2)
        limpar_tela()
            
    arquivo_dados = open("dados.txt", "w")
    json.dump(dados, arquivo_dados, indent=4)
    arquivo_dados.close()

def descurtir_m(user,nome_musica):
    arquivo_dados=open("dados.txt", "r")
    dados=json.load(arquivo_dados)
    arquivo_dados.close()

    print("\n=== DESCURTIR MÚSICA ===\n")

    print("=== MÚSICAS JÁ DESCURTIDAS ===\n")

    for curtida in dados["historico"]["nao_curtidas"]:
            if curtida["usuario"] == user:
                print("{} - {} ({})\n".format(curtida["musicas"]["musica"], curtida["musicas"]["artista"], curtida["musicas"]["duracao"]))
                break

    musica_descurtida=[]
    for musica in dados["musicas"]:
        if musica["nome"].upper() == nome_musica:
            time.sleep(1)
            print("Música encontrada!")
            time.sleep(1)
            print("{} - {} - ({})".format(musica["nome"], musica["artista"], musica["duracao"]))
            musica_descurtida = {
                "musica": musica["nome"],
                "artista": musica["artista"],
                "duracao": musica["duracao"]
                }
    if musica_descurtida:
        dados["historico"]["nao_curtidas"].append({
            "usuario": user,
            "musicas": musica_descurtida,
            })
        print("Música não curtida com sucesso!")
        time.sleep(2)
        limpar_tela()

    arquivo_dados = open("dados.txt", "w")
    json.dump(dados, arquivo_dados, indent=4)
    arquivo_dados.close()


def retirar_c_d(user):

    arquivo_dados=open("dados.txt", "r")
    dados=json.load(arquivo_dados)
    arquivo_dados.close()

    print("=== RETIRAR CURTIDA OU DESCURTIDA ===")

    print("\n1. Retirar Curtida\n2. Retirar Descurtida\n3.Voltar")

    escolha=input("\nOpção: ")

    time.sleep(1)
    limpar_tela()

    
    if escolha == "1":
        print("=== RETIRAR CURTIDA ===")

        for curtida in dados["historico"]["curtidas"]:
            if curtida["usuario"] == user:
                print("{} - {} ({})".format(curtida["musicas"]["musica"], curtida["musicas"]["artista"], curtida["musicas"]["duracao"]))
                break
        
        nome_musica = str(input("Digite o nome da música: ")).upper().strip()
        for musica in dados["historico"]["curtidas"]:
            if musica["musicas"]["musica"].upper() == nome_musica and musica["usuario"] == user:
                print("Música encontrada!")

                time.sleep(1)
                
                
                
                dados["historico"]["curtidas"].remove(musica)
                print("Música removida com sucesso!")
                arquivo_dados = open("dados.txt", "w")
                json.dump(dados, arquivo_dados, indent=4)
                arquivo_dados.close()
                time.sleep(2)
                limpar_tela()
                
                
                break
    

    if escolha == "2":
        print("=== RETIRAR DESCURTIDA ===")


        for curtida in dados["historico"]["nao_curtidas"]:
            if curtida["usuario"] == user:
                print("{} - {} ({})".format(curtida["musicas"]["musica"], curtida["musicas"]["artista"], curtida["musicas"]["duracao"]))
                break
        
        nome_musica = str(input("Digite o nome da música: ")).upper().strip()
        for musica in dados["historico"]["nao_curtidas"]:
            if musica["musicas"]["musica"].upper() == nome_musica and musica["usuario"] == user:
                print("Música encontrada!")

                time.sleep(1)
                
                
                
                dados["historico"]["nao_curtidas"].remove(musica)
                print("Música removida com sucesso!")
                arquivo_dados = open("dados.txt", "w")
                json.dump(dados, arquivo_dados, indent=4)
                arquivo_dados.close()
                time.sleep(2)
                limpar_tela()
                
                
                break


def buscar_musicas(user):

    arquivo_dados = open("dados.txt", "r")
    dados = json.load(arquivo_dados)
    arquivo_dados.close()

    print("=== BUSCAR MÚSICA ===")
    nome_musica = str(input("Digite o nome da música: ")).upper().strip()

    for musica in dados["musicas"]:
        if musica["nome"].upper() == nome_musica:
            time.sleep(1)
            print("Música encontrada!")
            time.sleep(1)
            print("{} - {} - ({})\n".format(musica["nome"], musica["artista"], musica["duracao"]))
            break

    while True:
        c_ou_dc=str(input("Curtir música? (S/N): "))
        time.sleep(2)
        limpar_tela()
        if c_ou_dc=="S":
            curtir_m(user,nome_musica)
            break
        elif c_ou_dc == "N":
            descurtir_m(user,nome_musica)
            break
        if musica["nome"].upper() != nome_musica:
            
            print("Música não encontrada!")
            
            break
        else:
           print("Digite S ou N apenas")
           
    arquivo_dados.close()


def criar_playlist(user):

    arquivo_dados = open("dados.txt", "r")
    dados = json.load(arquivo_dados)
    arquivo_dados.close()

    print("=== CRIAR PLAYLIST ===")

    nome_playlist=str(input("\nDigite o nome da playlist: ")).upper().strip()
        
    


    nova_playlist=[]
    while True:

            for playlists in dados["playlists"]:
                if playlists["playlist"].upper() == nome_playlist:
                    time.sleep(1)
                    print("Playlist já existente!")
                    time.sleep(1)
                    limpar_tela()
                    gerenciar_playlist(user)
                    break

                    
            nome_musica = str(input("Digite o nome da música (ENTER para finalizar): ")).upper().strip()   
            
            if nome_musica=="":
                break

            for musica in dados["musicas"]:
        
                if musica["nome"].upper() == nome_musica:
                    time.sleep(1)
                    print("Música encontrada!")
                    time.sleep(1)
                    print("{} - {} - ({})".format(musica["nome"], musica["artista"], musica["duracao"]))
                    time.sleep(2)
                    nova_playlist.append({
                    "musica": musica["nome"],
                    "artista": musica["artista"],
                    "duracao": musica["duracao"]
                })
                 
    if nova_playlist:
        dados["playlists"].append({
        "usuario": user,
        "playlist": nome_playlist,
        "musicas": nova_playlist
    })
        
    arquivo_dados = open("dados.txt", "w")
    json.dump(dados, arquivo_dados, indent=4)
    arquivo_dados.close()

    print("Playlist criada com sucesso!")
    time.sleep(2)
    limpar_tela()
    
    
def editar_playlist(user):

    arquivo_dados = open("dados.txt", "r")
    dados = json.load(arquivo_dados)
    arquivo_dados.close()

    print("=== EDITAR PLAYLIST ===")
    nome_playlist = str(input("Digite o nome da playlist que deseja EDITAR: ")).upper().strip()
    

    for playlists in dados["playlists"]:
        if playlists["playlist"].upper() == nome_playlist and user ==playlists["usuario"]:
            time.sleep(1)
            print("Playlist encontrada!")
            time.sleep(1)
            limpar_tela()

            print("=== EDITAR PLAYLIST ===")
            
            print("\n1. Adicionar Música\n2. Remover Música\n3.Voltar")

            escolha=input("\nOpção: ")
            
            time.sleep(1)
            limpar_tela()
            

            if escolha == '1':
                limpar_tela()

                for playlist in dados["playlists"]:
                    if playlist["usuario"] == user and playlist["playlist"].upper() == nome_playlist:
                       print("Músicas da playlist --> {}: ".format(nome_playlist))
                       for musica in playlist["musicas"]:
                           print("{} - {} ({})".format(musica["musica"], musica["artista"], musica["duracao"]))


                nome_musica=str(input("Digite o nome da música que deseja ADICIONAR: ")).upper().strip()

                for musica in dados["musicas"]:
                    if musica["nome"].upper() == nome_musica:
                      time.sleep(1)
                      print("Música encontrada!")
                      time.sleep(1)
                      print("{} - {} - ({})".format(musica["nome"], musica["artista"], musica["duracao"]))
                      musica_encontrada = {
                            "musica": musica["nome"],
                            "artista": musica["artista"],
                            "duracao": musica["duracao"]
                        }
                      break

            if escolha == "2":

                for playlist in dados["playlists"]:
                    if playlist["usuario"] == user and playlist["playlist"].upper() == nome_playlist:
                       print("Músicas da playlist {}".format(nome_playlist))
                       for musica in playlist["musicas"]:
                           print("{} - {} ({})".format(musica["musica"], musica["artista"], musica["duracao"]))

                nome_musica=str(input("Digite o nome da música que deseja REMOVER: ")) 

                for playlist in dados["playlists"]:
                    if playlist["usuario"] == user and playlist["playlist"].upper() == nome_playlist:
                        for musica in playlist["musicas"]:
                            if musica["musica"].upper() == nome_musica:
                                playlist["musicas"].remove(musica)
                                print("Música removida com sucesso!")
                                
                                
                                break

                    arquivo_dados = open("dados.txt", "w")
                    json.dump(dados, arquivo_dados, indent=4)
                    arquivo_dados.close()


            if escolha=="1" and musica_encontrada:
               
               playlists["musicas"].append(musica_encontrada)
               print("Playlist editada com sucesso!")
               time.sleep(2)
               limpar_tela()
               

               arquivo_dados = open("dados.txt", "w")
               json.dump(dados, arquivo_dados, indent=4)
               arquivo_dados.close()
               break
            
            if escolha == "3":
                break


def excluir_playlist(user):

    arquivo_dados = open("dados.txt", "r")
    dados = json.load(arquivo_dados)
    arquivo_dados.close()

    print("=== EXCLUIR PLAYLIST ===")

    nome_playlist=str(input("Digite o nome da playlist: ")).upper().strip()

    for playlists in dados["playlists"]:
        if playlists["playlist"].upper() == nome_playlist and user ==playlists["usuario"]:
            print("Playlist encontrada!")
            
            
            dados["playlists"].remove(playlists)
            print("Playlist removida com sucesso!")
            
            

            arquivo_dados = open("dados.txt", "w")
            json.dump(dados, arquivo_dados, indent=4)
            arquivo_dados.close()

            
def gerenciar_playlist(user):

    print("=== GERENCIAR PLAYLISTS ===")

    print("\n1. Criar Playlist\n2. Editar Playlists\n3. Excluir Playlist\n4. Voltar")

    oq_fazer = input("\nOpção: ")
    
    time.sleep(2)
    limpar_tela()

    while True:
        if oq_fazer == '1':
         criar_playlist(user)
         break
        if oq_fazer == '2':
         editar_playlist(user)
         break
        if oq_fazer == '3':
         excluir_playlist(user)
         break
        if oq_fazer == '4':
         break
    
#Menu secundario 
def menu_playlist(user):
    
    while True:
        print("=== BEM-VINDO AO SPOTIFEI ===")
        print("\n1. Buscar músicas\n2. Gerenciar playlists\n3. Visualizar o histórico\n4. Retirar Curtidas e Descurtidas\n5. Sair")

        escolha = input("\nOpção: ")
        
        time.sleep(2)
        limpar_tela()

        if escolha == "1":
            buscar_musicas(user)  
            
        elif escolha == "2":
            gerenciar_playlist(user)
                

        elif escolha == "3":
            vizualizar_hist(user)     
            
        elif escolha == "4":
            retirar_c_d(user)
        elif escolha == "5":
            print("=== VOLTANDO AO MENU INICIAL ===")
            time.sleep(2)
            limpar_tela()
            
            break
        else:
            print("Opção inválida. Digite de 1 a 4.")


#Menu inicial
while True:
    print("=== MENU INICIAL ===")
    print("1. Cadastrar")
    print("2. Fazer login")
    print("3. Sair")

    opcao = input("\nOpção: ")
    
    time.sleep(2)
    limpar_tela()
    if opcao == "1":
        if cadastro() == True:
            user = login()
            if user:
              menu_playlist(user)

    elif opcao == "2":
      user = login()
      if user:
        menu_playlist(user)



    elif opcao == "3":
        print("=== FECHANDO O APLICATIVO ===")
        time.sleep(1)
        limpar_tela()
        
        break
    else:
        print("Opção inválida. Digite 1, 2 ou 3.")
