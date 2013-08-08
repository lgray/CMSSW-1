import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.options = cms.untracked.PSet(
    SkipEvent = cms.untracked.vstring('ProductNotFound')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1) # Don't change this!!!!!!
)

process.source = cms.Source("EmptySource")

process.demo = cms.EDAnalyzer('PatternExtractor', inputRootFile = cms.string('file:/scratch/jets.root'),inputBankFile = cms.string('/home/infor/baulieu/private/cms/CMSSW_6_1_2_SLHC3/src/amsimulation/612_SLHC6_MUBANK_low_sec37_ss32_cov40_threshPT3.pbk'), threshold = cms.int32(5))


process.p = cms.Path(process.demo)
