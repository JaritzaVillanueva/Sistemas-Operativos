typedef struct {
    char jump[3];  // saltos de ejecucion 
    char FSName[8]; // Nombre del sistema de archivos
    char Chunk[53]; // Datos reservados: Tamaño del sector, sectores por clúster, número de FATs, etc.
    long offsetPart; // Offset de la partición
    long sizeVol; // Tamaño de la partición
    int FATOffset; // Offset de la FAT
    int FATlen; // Tamaño de la FAT
    int ClusterHeapOffset; // Offset del área de datos
    int ClusterCount; // Número de clústeres en el área de datos
    int RootDirFirstCluster; // Clúster inicial del directorio raíz
    int VSN; // Número de serie del volumen
    short FSR; // Tamaño del sector de la FAT
    short FlagVol; // Flags del volumen
    char BytePerSector; // Bytes por sector
    char SectorPerCluster; // Sectores por clúster
    char NumberFats; // Número de FATs
    char DriveSlect; // Selector de unidad
    char PercentUse; // Porcentaje de uso del volumen
    char reserved[7]; // Reservados
    char BootCode[390]; // Código de arranque
    short BootSignature; // Firma de arranque (0xAA55)
} __attribute((packed)) exFatBootSector;