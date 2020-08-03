﻿#pragma once

#include "StyleTransferEffect.g.h"

using namespace winrt::Windows::Media::Effects;
using namespace winrt::Windows::Media::MediaProperties;
using namespace winrt::Windows::Graphics::DirectX::Direct3D11;
using namespace winrt::Windows::Foundation::Collections;
using namespace winrt::Microsoft::AI::MachineLearning;
using namespace winrt::Windows::Media;

namespace winrt::StyleTransferEffectCpp::implementation
{
	struct SwapChainEntry {
		LearningModelBinding binding;
		Windows::Foundation::IAsyncOperation<LearningModelEvaluationResult> activetask;
		VideoFrame outputCache;
		SwapChainEntry() :
			binding(nullptr),
			activetask(nullptr),
			outputCache(VideoFrame(Windows::Graphics::Imaging::BitmapPixelFormat::Bgra8, 720, 720)) {}
	};

	struct StyleTransferEffect : StyleTransferEffectT<StyleTransferEffect>
	{
		StyleTransferEffect();
		IVectorView<VideoEncodingProperties> SupportedEncodingProperties();
		bool TimeIndependent();
		MediaMemoryTypes SupportedMemoryTypes();
		bool IsReadOnly();
		IPropertySet configuration;

		void DiscardQueuedFrames();
		void Close(MediaEffectClosedReason);
		void ProcessFrame(ProcessVideoFrameContext);
		void SetEncodingProperties(VideoEncodingProperties, IDirect3DDevice);
		void SetProperties(IPropertySet);
		void SubmitEval(VideoFrame, VideoFrame);

	private:
		VideoEncodingProperties encodingProperties;
		std::mutex Processing;
		StyleTransferEffectNotifier Notifier;
		LearningModelSession Session;
		Windows::Foundation::IAsyncOperation<LearningModelEvaluationResult> evalStatus;
		hstring InputImageDescription;
		hstring OutputImageDescription;
		int swapChainIndex = 0;
		static const int swapChainEntryCount = 5;
		std::vector < std::unique_ptr<SwapChainEntry>> bindings;
		int finishedIdx = 0;
	};
}

namespace winrt::StyleTransferEffectCpp::factory_implementation
{
	struct StyleTransferEffect : StyleTransferEffectT<StyleTransferEffect, implementation::StyleTransferEffect>
	{
	};
}
