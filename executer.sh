#! /bin/bash
    
if [ $# -ne 4 ]
then
    echo "Parametres manquants. Relancez comme suit :"
    echo  "$0 nbr_Lignes nbr_Colonnes nbr_Travailleurs nbr_Repetitions_Test"
    exit 1
fi

logF="log_N"$1"_M"$2"_NbTrav"$3"_nbTst"$4

rm $logF
echo "Les tests commencent... les traces seront dans " $logF
for i in `seq 1 $4`
do
    ./bin/tpnote $1 $2 $3 >> $logF
    echo "Fin un test" >> $logF
done

exit 0
