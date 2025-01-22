# barcode-pbm-ean8
A set of tools for generating and reading EAN-8 barcodes in PBM (Portable Bitmap) format. This project was developed as part of the Introduction to Programming Techniques (ITP) course at UFRN (Federal University of Rio Grande do Norte).
Portuguese Version / Versão em Português
Overview
This project implements a complete solution for working with EAN-8 barcodes, allowing both generation and reading of barcodes in the PBM format. The EAN-8 format is a shortened version of the EAN-13 barcode system, commonly used for smaller product packaging.
Features

Generate EAN-8 barcodes as PBM images
Read and decode EAN-8 barcodes from PBM images
Barcode validation with checksum verification
Customizable barcode dimensions and margins
Command-line interface for easy usage

Components

generatebc: Program to generate barcode PBM images
readbc: Program to read barcodes from PBM images
barcode.h: Header file with core functions
barcode.c: Implementation of barcode operations

Building the Project
gcc -o generatebc generatebc.c barcode.c -I.
gcc -o readbc readbc.c barcode.c -I.
Usage
Generating Barcodes
./generatebc <barcode_number> [margin_size] [bar_width] [bar_height] [output_path]
Parameters:

barcode_number: 8-digit EAN-8 barcode number
margin_size: White margin size in pixels (default: 4)
bar_width: Width of each bar in pixels (default: 3)
bar_height: Height of the barcode in pixels (default: 50)
output_path: Output file path (default: barcode_output.pbm)

Reading Barcodes
./readbc <input_file>
Parameters:

input_file: Path to the PBM file containing the barcode

Technical Details

Uses PBM (Portable Bitmap) format for maximum compatibility
Implements EAN-8 encoding patterns for left and right digit groups
Includes checksum validation
Supports customizable dimensions and margins
Error handling for invalid inputs and file operations


<a name="português"></a>
barcode-pbm-ean8
Um conjunto de ferramentas para gerar e ler códigos de barras EAN-8 no formato PBM (Portable Bitmap). Este projeto foi desenvolvido como parte da disciplina de Introdução às Técnicas de Programação (ITP) da UFRN (Universidade Federal do Rio Grande do Norte).
Visão Geral
Este projeto implementa uma solução completa para trabalhar com códigos de barras EAN-8, permitindo tanto a geração quanto a leitura de códigos de barras no formato PBM. O formato EAN-8 é uma versão reduzida do sistema de código de barras EAN-13, comumente usado em embalagens menores de produtos.
Funcionalidades

Gera códigos de barras EAN-8 como imagens PBM
Lê e decodifica códigos de barras EAN-8 de imagens PBM
Validação de código de barras com verificação de checksum
Dimensões e margens personalizáveis
Interface de linha de comando para fácil utilização

Componentes

generatebc: Programa para gerar imagens PBM de códigos de barras
readbc: Programa para ler códigos de barras de imagens PBM
barcode.h: Arquivo de cabeçalho com funções principais
barcode.c: Implementação das operações de código de barras

Compilação do Projeto
gcc -o generatebc generatebc.c barcode.c -I.
gcc -o readbc readbc.c barcode.c -I.
Uso
Gerando Códigos de Barras
./generatebc <numero_codigo> [tamanho_margem] [largura_barra] [altura_barra] [caminho_saida]
Parâmetros:

numero_codigo: Número EAN-8 de 8 dígitos
tamanho_margem: Tamanho da margem branca em pixels (padrão: 4)
largura_barra: Largura de cada barra em pixels (padrão: 3)
altura_barra: Altura do código de barras em pixels (padrão: 50)
caminho_saida: Caminho do arquivo de saída (padrão: barcode_output.pbm)

Lendo Códigos de Barras
./readbc <arquivo_entrada>
Parâmetros:

arquivo_entrada: Caminho para o arquivo PBM contendo o código de barras

Detalhes Técnicos

Utiliza o formato PBM (Portable Bitmap) para máxima compatibilidade
Implementa padrões de codificação EAN-8 para grupos de dígitos esquerdos e direitos
Inclui validação de checksum
Suporta dimensões e margens personalizáveis
Tratamento de erros para entradas inválidas e operações com arquivos
