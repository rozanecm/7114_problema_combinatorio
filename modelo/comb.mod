# conjuntos
##set ESTADOS := {1..48};
set ESTADOS;
set s dimen 2;
set s1;

# Parametros (constantes)
# Distancias entre ESTADOS
param Dij{i in ESTADOS, j in ESTADOS};

# DA i : variable continua que indica la distancia del datacenter A al estado
# i. (ı́dem para datacenter B)
param DAi{i in ESTADOS};
param DBi{i in ESTADOS};

# M : valor mayor a cualquier distancia posible.
param M := 10000;


# all distances
table tab_distances IN "CSV" "distances.csv" : s <- [state1, state2], Dij~distance;

# distances from datacenters A and B, which correspond to states 
# of Florida and Oregon respectively.
table tab_florida_distances IN "CSV" "florida_distances.csv" : ESTADOS <- [state2], DAi~distance;
table tab_oregon_distances IN "CSV" "oregon_distances.csv" : s1 <- [state2], DBi~distance;

# Variables
# Li: variable continua que indica la latencia correspondiente al estado i
var Li{i in ESTADOS} >= 0;

# DC i : variable continua que indica la distancia del datacenter C 
# al estado i. Idem para D y E.
var DCi{i in ESTADOS} >= 0;
var DDi{i in ESTADOS} >= 0;
var DEi{i in ESTADOS} >= 0;

# YAi: variable bivalente que vale 0 cuando la distancia del Datacenter
# A al Estado i es la menor respecto al resto de los Datacenters. (ı́dem
# B, C, D, E)
var YAi{i in ESTADOS} >= 0, binary;
var YBi{i in ESTADOS} >= 0, binary;
var YCi{i in ESTADOS} >= 0, binary;
var YDi{i in ESTADOS} >= 0, binary;
var YEi{i in ESTADOS} >= 0, binary;

# Y Ce i : variable bivalente que vale 1 cuando el datacenter C está en el
# Estado i.
var YCei{i in ESTADOS} >= 0, binary;
var YDei{i in ESTADOS} >= 0, binary;
var YEei{i in ESTADOS} >= 0, binary;

# Restricciones
# Cada L i tendrá como cota superior la distancia al datacenter más próximo, y
# como cota inferior esa misma distancia en el caso de que el estado i tenga la
# menor distancia.
s.t. cota_sup_dcA{i in ESTADOS}: Li[i] <= DAi[i];
s.t. cota_sup_dcB{i in ESTADOS}: Li[i] <= DBi[i];
s.t. cota_sup_dcC{i in ESTADOS}: Li[i] <= DCi[i];
s.t. cota_sup_dcD{i in ESTADOS}: Li[i] <= DDi[i];
s.t. cota_sup_dcE{i in ESTADOS}: Li[i] <= DEi[i];

s.t. cota_inf_dcA{i in ESTADOS}: Li[i] >= DAi[i] - M * YAi[i];
s.t. cota_inf_dcB{i in ESTADOS}: Li[i] >= DBi[i] - M * YBi[i];
s.t. cota_inf_dcC{i in ESTADOS}: Li[i] >= DCi[i] - M * YCi[i];
s.t. cota_inf_dcD{i in ESTADOS}: Li[i] >= DDi[i] - M * YDi[i];
s.t. cota_inf_dcE{i in ESTADOS}: Li[i] >= DEi[i] - M * YEi[i];

# sumatorioa de YXi debe ser 4
s.t. total_datacenters{i in ESTADOS}: YAi[i] + YBi[i] + YCi[i] + YDi[i] + YEi[i] = 4;

# Asociacion de datacenter a estado
s.t. asoc_dcC: sum{i in ESTADOS} YCei[i] = 1;
s.t. asoc_dcD: sum{i in ESTADOS} YDei[i] = 1;
s.t. asoc_dcE: sum{i in ESTADOS} YEei[i] = 1;

# Asociación de distancia de un estado al datacenter correspondiente
s.t. asoc_dist_dcC_estadoi{i in ESTADOS}: sum{(i,j) in s} Dij[i,j] * YCei[j] = DCi[i];
s.t. asoc_dist_dcD_estadoi{i in ESTADOS}: sum{(i,j) in s} Dij[i,j] * YDei[j] = DDi[i];
s.t. asoc_dist_dcE_estadoi{i in ESTADOS}: sum{(i,j) in s} Dij[i,j] * YEei[j] = DEi[i];

# funcional
minimize z: sum{i in ESTADOS} Li[i];

end;
