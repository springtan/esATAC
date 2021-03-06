% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/Methods.R
\docType{package}
\name{esATAC-package}
\alias{esATAC-package}
\alias{atacPipe}
\title{An Easy-to-use Systematic pipeline for ATACseq data analysis}
\usage{
atacPipe(genome, fastqInput1, fastqInput2 = NULL, refdir = NULL,
  tmpdir = NULL, threads = 2, adapter1 = NULL, adapter2 = NULL,
  interleave = FALSE, basicAnalysis = FALSE, createReport = TRUE,
  motifs = NULL, prefix = NULL, chr = c(1:22, "X", "Y"),
  p.cutoff = 1e-06, ...)
}
\arguments{
\item{genome}{\code{Character} scalar. The genome(like hg19, mm10, etc.) reference data in "refdir" to be used in the pipeline.}

\item{fastqInput1}{\code{Character} vector. For single-end sequencing,
it contains sequence file paths.
For paired-end sequencing, it can be file paths with #1 mates paired
with file paths in fastqInput2
And it can also be interleaved file paths when argument
interleaved=\code{TRUE}}

\item{fastqInput2}{\code{Character} vector. It contains file paths with #2
mates paired with file paths in fastqInput1.
For single-end sequencing files and interleaved paired-end sequencing
files(argument interleaved=\code{TRUE}),
it must be \code{NULL}.}

\item{refdir}{\code{Character} scalar. The path for reference data being installed to and storage.}

\item{tmpdir}{\code{Character} scalar. The temporary file storage path.}

\item{threads}{\code{Integer} scalar. The max threads allowed to be created.}

\item{adapter1}{\code{Character} scalar. It is an adapter sequence for file1.
For single end data, it is requied.}

\item{adapter2}{\code{Character} scalar. It is an adapter sequence for file2.}

\item{interleave}{\code{Logical} scalar. Set \code{TRUE} when files are
interleaved paired-end sequencing data.}

\item{basicAnalysis}{\code{Logical} scalar. If it is TRUE, the pipeline will skip the time consuming steps
like GO annoation and motif analysis}

\item{createReport}{\code{Logical} scalar. If the HTML report file will be created.}

\item{motifs}{either\code{\link{PFMatrix}}, \code{\link{PFMatrixList}},
\code{\link{PWMatrix}}, \code{\link{PWMatrixList}}, default: vertebrates motif from JASPAR.}

\item{prefix}{\code{Character} scalar. Temporary file prefix for identifying files
when multiple pipeline generating file in the same tempdir.}

\item{chr}{Which chromatin the program will processing. It must be identical
with the filename of cut site information files or subset of .
Default:c(1:22, "X", "Y").}

\item{p.cutoff}{p-value cutoff for returning motifs, default: 1e-6.}

\item{...}{Additional arguments, currently unused.}
}
\value{
\code{List} scalar. It is a list that save the result of the pipeline.
Slot "filelist": the input file paths.
Slot "wholesummary": a dataframe that for quality control summary
Slot "atacProcs": \code{\link{ATACProc-class}} objects generated by each process in the pipeline.
Slot "filtstat": a dataframe that summary the reads filted in each process.
}
\description{
This package provides a framework and complete preset pipeline for
the quantification and analysis of ATAC-seq Reads. It covers raw sequencing
reads preprocessing (FASTQ files), reads alignment (Rbowtie2), aligned reads
file operation (SAM, BAM, and BED files), peak calling (fseq), genome
annotations (Motif, GO, SNP analysis) and quality control report. The package
is managed by dataflow graph. It is easy for user to pass variables seamlessly
between processes and understand the workflow. Users can process FASTQ files
through end-to-end preset pipeline which produces a pretty HTML report for
quality control and preliminary statistical results, or customize workflow
starting from any intermediate stages with esATAC functions easily and flexibly.

Preset pipeline for single replicate case study is shown below.

For multi-replicates case study, see \code{\link{atacRepsPipe}}.

For single replicate case-control study, see \code{\link{atacPipe2}}.

For multi-replicates case-control study, see \code{\link{atacRepsPipe2}}.


NOTE:
Build bowtie index in the function may take some time.
If you already have bowtie2 index files or
you want to download(\url{ftp://ftp.ccb.jhu.edu/pub/data/bowtie2_indexes})
instead of building,
you can let esATAC skip the steps by renaming them following the format
(genome+suffix) and put them in reference installation path (refdir).
Example: hg19 bowtie2 index files

\itemize{
\item hg19.1.bt2
\item hg19.2.bt2
\item hg19.3.bt2
\item hg19.4.bt2
\item hg19.rev.1.bt2
\item hg19.rev.2.bt2
}

For single end reads FASTQ files,
The required parameters are fastqInput1 and adapter1.
For paired end reads non-interleaved FASTQ files (interleave=FALSE,defualt),
The required parameters are fastqInput1 and fastqInput2.
Otherwise, parameter fastqInput2 is not required (interleave=TRUE)

The paths of sequencing data replicates can be a \code{Character} vector.
For example:

fastqInput1=c("file_1.rep1.fastq","file_1.rep2.fastq")

fastqInput2=c("file_2.rep1.fastq","file_2.rep2.fastq")

The result will be return by the function.
An HTML report file will be created for paired end reads.
Intermediate files will be save at tmpdir path (default is ./)
}
\details{
See packageDescription('esATAC') for package details.
}
\examples{
\dontrun{
## These codes are time consuming so they will not be run and
## checked by bioconductor checker.


# call pipeline
# for a quick example(only CTCF and BATF3 will be processing)
conclusion <-
  atacPipe(
       # MODIFY: Change these paths to your own case files!
       # e.g. fastqInput1 = "your/own/data/path.fastq"
       fastqInput1 = system.file(package="esATAC", "extdata", "chr20_1.1.fq.gz"),
       fastqInput2 = system.file(package="esATAC", "extdata", "chr20_2.1.fq.gz"),
       # MODIFY: Set the genome for your data
       genome = "hg19",
       motifs = getMotifInfo(motif.file = system.file("extdata", "CustomizedMotif.txt", package="esATAC")))

# call pipeline
# for overall example(all vertebrates motif in JASPAR will be processed)
conclusion <-
  atacPipe(
       # MODIFY: Change these paths to your own case files!
       # e.g. fastqInput1 = "your/own/data/path.fastq"
       fastqInput1 = system.file(package="esATAC", "extdata", "chr20_1.1.fq.gz"),
       fastqInput2 = system.file(package="esATAC", "extdata", "chr20_2.1.fq.gz"),
       # MODIFY: Set the genome for your data
       genome = "hg19")
}
}
\seealso{
\code{\link{printMap}},
\code{\link{atacPipe2}},
\code{\link{atacRenamer}},
\code{\link{atacRemoveAdapter}},
\code{\link{atacBowtie2Mapping}},
\code{\link{atacPeakCalling}},
\code{\link{atacMotifScan}},
\code{\link{atacRepsPipe}},
\code{\link{atacRepsPipe2}}
}
\author{
Zheng Wei and Wei Zhang
}
