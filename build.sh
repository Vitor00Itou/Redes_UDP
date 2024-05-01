# Script para compilação e executação do programa

# Compilação do programa
echo "Compilando o programa..."

# Compilador + arquivos c++ + nome do arquvio + parametro para OTIMIZAÇÃO EXTREMA da compilação
g++ source/*.cpp -o jvav_mensages -O3 

# Verifica se houve erro na compilação
if [ $? -eq 0 ]; then
    echo "Compilação bem-sucedida. Executando o programa..."
    # Execução do programa
    ./jvav_mensages
else
    echo "Erro durante a compilação. O programa não será executado."
fi
