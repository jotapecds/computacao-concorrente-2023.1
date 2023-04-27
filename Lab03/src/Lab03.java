/* Disciplina: Computacao Concorrente */
/* Universidade Federal do Rio de Janeiro */
/* Computação concorrente: Laboratório 3 */
/* Autor: João Pedro Coelho de Souza */

/* Codigo: Encontrando aproximações de PI
   através da implementação de uma versão
   concorrente de uma Série de Taylor */

class MinhaAproximacaoDePi {
    private double[] series;

    public MinhaAproximacaoDePi(int nSeries){ this.series = new double[nSeries]; }

    public double GetSerie(int iSerie) { return series[iSerie]; }
    public void setSerie(int iSerie, double valor) { series[iSerie] = valor; }

    public double GetAproximacao() {
        double total=0;
        // Somatório de todas as séries calculadas
        for (double v : series) {
            total += v;
        }
        return total;
    }

    // Debug
    public void PrintSeries() {
        for (int i = 0; i < series.length; i++) {
            System.out.println("Serie["+i+"] = "+ series[i]);
        }
    }
}

class CalculaSerie extends Thread {
    private MinhaAproximacaoDePi pi;    // Referência ao objeto de Pi Que está sendo calculado
    private int nSeries;
    private int idThread;
    private int nThreads;

    public CalculaSerie(MinhaAproximacaoDePi _pi, int _nSeries, int _idThread, int _nThreads) {
        this.pi = _pi;
        this.nSeries = _nSeries;
        this.idThread = _idThread;
        this.nThreads = _nThreads;
    }

    //--metodo executado pela thread
    public void run() {
        for (int i=idThread; i<nSeries; i+=nThreads){
            pi.setSerie(i, Math.pow(-1, i)/(2*i + 1)*4);    // Cálculo da série
        }
    }
}

public class Lab03 {
    static final int NTHREADS = 2;
    static final int NSERIES = 10000000;

    public static void main(String[] args) {
        Thread[] threads = new Thread[NTHREADS];
        MinhaAproximacaoDePi meuPi = new MinhaAproximacaoDePi(NSERIES);

        for (int iThread=0; iThread<threads.length; iThread++) {
            threads[iThread] = new CalculaSerie(meuPi, NSERIES, iThread, NTHREADS);
            threads[iThread].start();
        }

        for (Thread thread : threads) {
            try { thread.join(); }
            catch (InterruptedException e) { return; }
        }

        double valorFinal = meuPi.GetAproximacao();
        //meuPi.PrintSeries();
        System.out.println("> Minha aproximação de Pi com "+ NSERIES +" séries: " + valorFinal);
        System.out.println(" (!) Aproximação "+(100*valorFinal)/(Math.PI)+"% compatível com Math.PI");
    }
}
