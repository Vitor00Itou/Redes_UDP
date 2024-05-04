# Script para compilação e executação do programa

# Compilação do programa
echo "Compilando o programa..."

# Compilador + arquivos c++ + nome do arquvio + parametro para OTIMIZAÇÃO EXTREMA da compilação
g++ source/UDP/*.cpp source/*.cpp -o jvav_UDP_messages -O3
g++ source/RAW/*.cpp source/*.cpp -o jvav_RAW_messages -O3

# Verifica se houve erro na compilação
if [ $? -eq 0 ]; then
    echo "Compilação bem-sucedida."
else
    echo "Erro durante a compilação."
fi
