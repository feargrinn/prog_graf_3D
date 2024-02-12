# Total 111/147

# House 2/2

16.10.23 OK

# Colors 5/5

16.10.23 OK

# Indices 5/5

23.10.2023 10:50:12 OK
Ale proszę usunąć wykomentowane wierzchołki.

# Uniforms 8/8

23.10.2023 10:52:15
Nie kompiluje się.

30.10.2023 11:04:06 OK

# PVM 5/5

6.11.2023 11:08:32 OK

# Pyramid 4/4

6.11.2023 11:08:52 OK

# Resize 8/8

13.11.2023 11:59:53 OK

# Zoom 10/10

13.11.2023 12:00:16
Nie kompiluje się. 

20.11.2023 17:36:30 OK 

# Camera movement 15/15 

27.11.2023 10:37:57 OK

# Mesh 10/10

13.12.2023 10:36:51
Proszę usunąć niepotrzebny wykomentowany kod. 

15.01.2024 16:31:53 ok

# Kd material 19/20

8.01.2024 15:52:24
Brak konstruktora który pozwoliłby ustawić wartość zmiennej `use_vertex_colors` bez ustawiania tekstury. 

15.01.2024 16:25:33
'Ify” w shaderze fragmentów są złe. Drugi ‘if’ nadpisuje zmienną `color` i zmienna `use_vertex_colors` nie ma znaczenia. 

22.01.2024 08:39:14 OK

# Textures 15/15 

8.01.2024 15:54:19
Proszę usunąć niepotrzebny wykomentowany kod. 

15.01.2024 16:28:50 OK

# OBJ reader 5/5

15.01.2024 16:29:31 OK

# Diffuse 20/25

22.01.2024 10:06:15
Uniform buffer w BlinnPhing material jest za mały. 


29.01.2024 13:21:16
Żle Pan przesyła wartości do bufora material. Zaczyna Pan od floata, a potem przesyła Pan vec4. Wedłu std140 vec4 powinien się zaczynać na granicy rozmiaru 4 floatów. Dlatego pojawia się Panu czerwony kwadrat

31.01.2024 11:16:43
normalna w ogóle nie jest używana do obliczenia swiatła!
Warunek sprawdzający czy ścianka jest front czy backfacing jest niepoprawny. Jakby Pan przestawił switalo na druga strone to ta scianka powinna być oświetlona. To się osiąga zmieniając normalne, takl jak to opisałem. 

12.02.2024 11:51:33 OK

# Specular 0/10

12.02.2024 11:52:09
view vector ma zły znak. 


