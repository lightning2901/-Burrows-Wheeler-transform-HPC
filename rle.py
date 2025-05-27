#! /usr/bin/env python3
def run_length_encode(data):
    encoded = []
    count = 1
    for i in range(1, len(data)):
        if data[i] == data[i - 1]:
            count += 1
        else:
            encoded.append(f"{count}{data[i - 1]}")
            count = 1
    encoded.append(f"{count}{data[-1]}")  # Último carácter
    return "".join(encoded)

arch = input("Hola, escribe el nombre de archivo de salida de la bwt: ")
output_name = input("Ahora el nombre con el que te gustaría guardarlo: ")
# Leer archivo BWT (ejemplo: "annb$aa")
with open(arch, "r", encoding = "latin1") as file:
    bwt_data = file.read().strip()

# Aplicar RLE
rle_data = run_length_encode(bwt_data)

# Guardar resultado
with open(output_name, "w", encoding = "utf-8") as file:
    file.write(rle_data)

#print(f"BWT original: {bwt_data}")
print(f"RLE resultante: {rle_data}")
