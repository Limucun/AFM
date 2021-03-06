#include "tool_base.h"


ToolBase::ToolBase()
{
    imageXOffset=0;
    imageYOffset=0;
    imageXLength=0;
    imageYLength=0;
    xSize=1;
    ySize=1;
    xScale=1.0;
    yScale=1.0;
}


QVector<QVector<double> > ToolBase::ToolCrossLine(double **matrix, int lastX, int lastY, int endX, int endY)
{

    QVector<QVector<double> > res(2);
    int x1 = (lastX-imageXOffset)/(double)imageXLength* xSize;
    int y1 = ((imageYOffset-lastY))/(double)imageYLength*ySize;
    int x2 = (endX-imageXOffset)/(double)imageXLength* xSize;
    int y2 = ((imageYOffset-endY))/(double)imageYLength*ySize;


    for(int p = qMin(x1, x2); p < qMin(x1, x2) + qAbs(x1 - x2); p++)
    {
        int tmpLine;
        tmpLine = (y2-y1)/(double)(x2-x1)*(p-x1)+y1;
        if(tmpLine>=0&&p>=0 &&tmpLine<ySize && p<xSize){
            res[0].push_back(p);
            res[1].push_back( matrix[tmpLine][p]);
        }
    }
    return res;
}


QVector<QVector<double> >ToolBase::ToolHPowerSpectralDensity(double **matrix){
    int N = xSize;
    QVector<QVector<double> > res(2, QVector<double>(xSize/2,0));
    double *in  = (double*) fftw_malloc(sizeof(double)*N);
    fftw_complex *out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
    for(int i=0; i<N/2; i++){
        double axe= 0.5*xSize/(xScale)- i/(xScale);
        axe = log(axe);
        res[0][i]=axe;
    }
    for(int j = 0; j<ySize; j++){
        for(int i=0; i<N; i++) in[i]= matrix[j][i];
        fftw_plan p = fftw_plan_dft_r2c_1d(N, in,out, FFTW_ESTIMATE);
        fftw_execute(p);
        for(int i=0; i<N/2; i++){
            double value = (xScale/xSize)/xSize*qAbs(out[0][i]*out[0][i]+out[1][i]*out[1][i]);
            value = log(value);
            res[1][xSize/2-1-i] += value;
        }
        fftw_destroy_plan(p);
    }
    for(int i=0; i<xSize/2; i++) res[1][i] /= ySize;
    fftw_free(out);
    return res;

}

QVector<QVector<double> >ToolBase::ToolVPowerSpectralDensity(double **matrix){
    int N = ySize;
    QVector<QVector<double> >res(2, QVector<double>(ySize/2,0));
    double *in = (double*) fftw_malloc(sizeof(double)*N);
    fftw_complex *out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
    for(int i=0; i<N/2; i++){
        double axe= 0.5*ySize/(xScale)- i/(xScale);
        axe = log(axe);
        res[0][i]=axe;
    }
    for(int j = 0; j<xSize; j++){
        for(int i=0; i<N; i++) in[i]= matrix[i][j];
        fftw_plan p = fftw_plan_dft_r2c_1d(N, in,out, FFTW_ESTIMATE);
        fftw_execute(p);
        for(int i=0; i<N/2; i++){
            double value = (xScale/xSize)/ySize*qAbs(out[0][i]*out[0][i]+out[1][i]*out[1][i]);
            value = log(value);
            res[1][N/2-1-i] += value;
        }
        fftw_destroy_plan(p);
    }
    for(int i=0; i<ySize/2; i++) res[1][i] /= xSize;
    fftw_free(out);
    return res;
}
