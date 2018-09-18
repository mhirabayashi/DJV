//------------------------------------------------------------------------------
// Copyright (c) 2004-2015 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#include <djvUI/CineonWidget.h>

#include <djvUI/FloatEditSlider.h>
#include <djvUI/UIContext.h>
#include <djvUI/IntEditSlider.h>
#include <djvUI/PrefsGroupBox.h>
#include <djvUI/Style.h>

#include <djvGraphics/ImageIO.h>

#include <djvCore/SignalBlocker.h>

#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QVBoxLayout>

namespace djv
{
    namespace UI
    {
        CineonWidget::CineonWidget(Graphics::ImageIO * plugin, UIContext * context) :
            ImageIOWidget(plugin, context)
        {
            //DJV_DEBUG("CineonWidget::CineonWidget");

            // Create the widgets.
            _inputColorProfileWidget = new QComboBox;
            _inputColorProfileWidget->addItems(Graphics::Cineon::colorProfileLabels());
            _inputColorProfileWidget->setSizePolicy(
                QSizePolicy::Fixed, QSizePolicy::Fixed);

            _inputBlackPointWidget = new IntEditSlider(context);
            _inputBlackPointWidget->setRange(0, 1023);

            _inputWhitePointWidget = new IntEditSlider(context);
            _inputWhitePointWidget->setRange(0, 1023);

            _inputGammaWidget = new FloatEditSlider(context);
            _inputGammaWidget->setRange(.01f, 4.f);

            _inputSoftClipWidget = new IntEditSlider(context);
            _inputSoftClipWidget->setRange(0, 50);

            _outputColorProfileWidget = new QComboBox;
            _outputColorProfileWidget->addItems(Graphics::Cineon::colorProfileLabels());
            _outputColorProfileWidget->setSizePolicy(
                QSizePolicy::Fixed, QSizePolicy::Fixed);

            _outputBlackPointWidget = new IntEditSlider(context);
            _outputBlackPointWidget->setRange(0, 1023);

            _outputWhitePointWidget = new IntEditSlider(context);
            _outputWhitePointWidget->setRange(0, 1023);

            _outputGammaWidget = new FloatEditSlider(context);
            _outputGammaWidget->setRange(.01f, 4.f);

            // Layout the widgets.
            _layout = new QVBoxLayout(this);
            _layout->setSpacing(context->style()->sizeMetric().largeSpacing);

            PrefsGroupBox * prefsGroupBox = new PrefsGroupBox(
                qApp->translate("djv::UI::CineonWidget", "Input Color Profile"),
                qApp->translate("djv::UI::CineonWidget", "Set the color profile used when loading Cineon images."),
                context);
            _inputColorProfileLayout = prefsGroupBox->createLayout();
            _inputColorProfileLayout->addRow(
                qApp->translate("djv::UI::CineonWidget", "Profile:"),
                _inputColorProfileWidget);
            _inputColorProfileLayout->addRow(
                qApp->translate("djv::UI::CineonWidget", "Black:"),
                _inputBlackPointWidget);
            _inputColorProfileLayout->addRow(
                qApp->translate("djv::UI::CineonWidget", "White:"),
                _inputWhitePointWidget);
            _inputColorProfileLayout->addRow(
                qApp->translate("djv::UI::CineonWidget", "Gamma:"),
                _inputGammaWidget);
            _inputColorProfileLayout->addRow(
                qApp->translate("djv::UI::CineonWidget", "Soft clip:"),
                _inputSoftClipWidget);
            _layout->addWidget(prefsGroupBox);

            prefsGroupBox = new PrefsGroupBox(
                qApp->translate("djv::UI::CineonWidget", "Output Color Profile"),
                qApp->translate("djv::UI::CineonWidget", "Set the color profile used when saving Cineon images."),
                context);
            _outputColorProfileLayout = prefsGroupBox->createLayout();
            _outputColorProfileLayout->addRow(
                qApp->translate("djv::UI::CineonWidget", "Profile:"),
                _outputColorProfileWidget);
            _outputColorProfileLayout->addRow(
                qApp->translate("djv::UI::CineonWidget", "Black:"),
                _outputBlackPointWidget);
            _outputColorProfileLayout->addRow(
                qApp->translate("djv::UI::CineonWidget", "White:"),
                _outputWhitePointWidget);
            _outputColorProfileLayout->addRow(
                qApp->translate("djv::UI::CineonWidget", "Gamma:"),
                _outputGammaWidget);
            _layout->addWidget(prefsGroupBox);

            _layout->addStretch();

            // Initialize.
            _inputBlackPointWidget->setDefaultValue(
                Graphics::Cineon::Options().inputFilmPrint.black);
            _inputWhitePointWidget->setDefaultValue(
                Graphics::Cineon::Options().inputFilmPrint.white);
            _inputGammaWidget->setDefaultValue(
                Graphics::Cineon::Options().inputFilmPrint.gamma);
            _inputSoftClipWidget->setDefaultValue(
                Graphics::Cineon::Options().inputFilmPrint.softClip);

            _outputBlackPointWidget->setDefaultValue(
                Graphics::Cineon::Options().outputFilmPrint.black);
            _outputWhitePointWidget->setDefaultValue(
                Graphics::Cineon::Options().outputFilmPrint.white);
            _outputGammaWidget->setDefaultValue(
                Graphics::Cineon::Options().outputFilmPrint.gamma);

            QStringList tmp;
            tmp = plugin->option(
                plugin->options()[Graphics::Cineon::INPUT_COLOR_PROFILE_OPTION]);
            tmp >> _options.inputColorProfile;
            tmp = plugin->option(
                plugin->options()[Graphics::Cineon::INPUT_FILM_PRINT_OPTION]);
            tmp >> _options.inputFilmPrint;
            tmp = plugin->option(
                plugin->options()[Graphics::Cineon::OUTPUT_COLOR_PROFILE_OPTION]);
            tmp >> _options.outputColorProfile;
            tmp = plugin->option(
                plugin->options()[Graphics::Cineon::OUTPUT_FILM_PRINT_OPTION]);
            tmp >> _options.outputFilmPrint;

            widgetUpdate();

            // Setup the callbacks.
            connect(
                plugin,
                SIGNAL(optionChanged(const QString &)),
                SLOT(pluginCallback(const QString &)));
            connect(
                _inputColorProfileWidget,
                SIGNAL(activated(int)),
                SLOT(inputColorProfileCallback(int)));
            connect(
                _inputBlackPointWidget,
                SIGNAL(valueChanged(int)),
                SLOT(inputBlackPointCallback(int)));
            connect(
                _inputWhitePointWidget,
                SIGNAL(valueChanged(int)),
                SLOT(inputWhitePointCallback(int)));
            connect(
                _inputGammaWidget,
                SIGNAL(valueChanged(float)),
                SLOT(inputGammaCallback(float)));
            connect(
                _inputSoftClipWidget,
                SIGNAL(valueChanged(int)),
                SLOT(inputSoftClipCallback(int)));
            connect(
                _outputColorProfileWidget,
                SIGNAL(activated(int)),
                SLOT(outputColorProfileCallback(int)));
            connect(
                _outputBlackPointWidget,
                SIGNAL(valueChanged(int)),
                SLOT(outputBlackPointCallback(int)));
            connect(
                _outputWhitePointWidget,
                SIGNAL(valueChanged(int)),
                SLOT(outputWhitePointCallback(int)));
            connect(
                _outputGammaWidget,
                SIGNAL(valueChanged(float)),
                SLOT(outputGammaCallback(float)));
            connect(
                context->style(),
                SIGNAL(sizeMetricsChanged()),
                SLOT(sizeMetricsCallback()));
        }

        CineonWidget::~CineonWidget()
        {
            //DJV_DEBUG("CineonWidget::~CineonWidget");
        }

        void CineonWidget::resetPreferences()
        {
            _options = Graphics::Cineon::Options();
            pluginUpdate();
            widgetUpdate();
        }

        void CineonWidget::pluginCallback(const QString & option)
        {
            try
            {
                QStringList tmp;
                tmp = plugin()->option(option);
                if (0 == option.compare(plugin()->options()[
                    Graphics::Cineon::INPUT_COLOR_PROFILE_OPTION], Qt::CaseInsensitive))
                    tmp >> _options.inputColorProfile;
                else if (0 == option.compare(plugin()->options()[
                    Graphics::Cineon::INPUT_FILM_PRINT_OPTION], Qt::CaseInsensitive))
                    tmp >> _options.inputFilmPrint;
                else if (0 == option.compare(plugin()->options()[
                    Graphics::Cineon::OUTPUT_COLOR_PROFILE_OPTION], Qt::CaseInsensitive))
                    tmp >> _options.outputColorProfile;
                else if (0 == option.compare(plugin()->options()[
                    Graphics::Cineon::OUTPUT_FILM_PRINT_OPTION], Qt::CaseInsensitive))
                    tmp >> _options.outputFilmPrint;
            }
            catch (const QString &)
            {
            }
            widgetUpdate();
        }

        void CineonWidget::inputColorProfileCallback(int in)
        {
            _options.inputColorProfile = static_cast<Graphics::Cineon::COLOR_PROFILE>(in);
            pluginUpdate();
        }

        void CineonWidget::inputBlackPointCallback(int in)
        {
            _options.inputFilmPrint.black = in;
            pluginUpdate();
        }

        void CineonWidget::inputWhitePointCallback(int in)
        {
            _options.inputFilmPrint.white = in;
            pluginUpdate();
        }

        void CineonWidget::inputGammaCallback(float in)
        {
            _options.inputFilmPrint.gamma = in;
            pluginUpdate();
        }

        void CineonWidget::inputSoftClipCallback(int in)
        {
            _options.inputFilmPrint.softClip = in;
            pluginUpdate();
        }

        void CineonWidget::outputColorProfileCallback(int in)
        {
            _options.outputColorProfile = static_cast<Graphics::Cineon::COLOR_PROFILE>(in);
            pluginUpdate();
        }

        void CineonWidget::outputBlackPointCallback(int in)
        {
            _options.outputFilmPrint.black = in;
            pluginUpdate();
        }

        void CineonWidget::outputWhitePointCallback(int in)
        {
            _options.outputFilmPrint.white = in;
            pluginUpdate();
        }

        void CineonWidget::outputGammaCallback(float in)
        {
            _options.outputFilmPrint.gamma = in;
            pluginUpdate();
        }

        void CineonWidget::sizeMetricsCallback()
        {
            _layout->setSpacing(context()->style()->sizeMetric().largeSpacing);
            updateGeometry();
        }

        void CineonWidget::pluginUpdate()
        {
            //DJV_DEBUG("CineonWidget::pluginUpdate");
            QStringList tmp;
            tmp << _options.inputColorProfile;
            plugin()->setOption(
                plugin()->options()[Graphics::Cineon::INPUT_COLOR_PROFILE_OPTION], tmp);
            tmp << _options.inputFilmPrint;
            plugin()->setOption(
                plugin()->options()[Graphics::Cineon::INPUT_FILM_PRINT_OPTION], tmp);
            tmp << _options.outputColorProfile;
            plugin()->setOption(
                plugin()->options()[Graphics::Cineon::OUTPUT_COLOR_PROFILE_OPTION], tmp);
            tmp << _options.outputFilmPrint;
            plugin()->setOption(
                plugin()->options()[Graphics::Cineon::OUTPUT_FILM_PRINT_OPTION], tmp);
        }

        void CineonWidget::widgetUpdate()
        {
            //DJV_DEBUG("CineonWidget::widgetUpdate");
            Core::SignalBlocker signalBlocker(QObjectList() <<
                _inputColorProfileWidget <<
                _inputBlackPointWidget <<
                _inputWhitePointWidget <<
                _inputGammaWidget <<
                _inputSoftClipWidget <<
                _outputColorProfileWidget <<
                _outputBlackPointWidget <<
                _outputWhitePointWidget <<
                _outputGammaWidget);
            _inputBlackPointWidget->setVisible(
                Graphics::Cineon::COLOR_PROFILE_AUTO == _options.inputColorProfile ||
                Graphics::Cineon::COLOR_PROFILE_FILM_PRINT == _options.inputColorProfile);
            _inputColorProfileLayout->labelForField(_inputBlackPointWidget)->setVisible(
                Graphics::Cineon::COLOR_PROFILE_AUTO == _options.inputColorProfile ||
                Graphics::Cineon::COLOR_PROFILE_FILM_PRINT == _options.inputColorProfile);
            _inputWhitePointWidget->setVisible(
                Graphics::Cineon::COLOR_PROFILE_AUTO == _options.inputColorProfile ||
                Graphics::Cineon::COLOR_PROFILE_FILM_PRINT == _options.inputColorProfile);
            _inputColorProfileLayout->labelForField(_inputWhitePointWidget)->setVisible(
                Graphics::Cineon::COLOR_PROFILE_AUTO == _options.inputColorProfile ||
                Graphics::Cineon::COLOR_PROFILE_FILM_PRINT == _options.inputColorProfile);
            _inputGammaWidget->setVisible(
                Graphics::Cineon::COLOR_PROFILE_AUTO == _options.inputColorProfile ||
                Graphics::Cineon::COLOR_PROFILE_FILM_PRINT == _options.inputColorProfile);
            _inputColorProfileLayout->labelForField(_inputGammaWidget)->setVisible(
                Graphics::Cineon::COLOR_PROFILE_AUTO == _options.inputColorProfile ||
                Graphics::Cineon::COLOR_PROFILE_FILM_PRINT == _options.inputColorProfile);
            _inputSoftClipWidget->setVisible(
                Graphics::Cineon::COLOR_PROFILE_AUTO == _options.inputColorProfile ||
                Graphics::Cineon::COLOR_PROFILE_FILM_PRINT == _options.inputColorProfile);
            _inputColorProfileLayout->labelForField(_inputSoftClipWidget)->setVisible(
                Graphics::Cineon::COLOR_PROFILE_AUTO == _options.inputColorProfile ||
                Graphics::Cineon::COLOR_PROFILE_FILM_PRINT == _options.inputColorProfile);
            _outputBlackPointWidget->setVisible(
                Graphics::Cineon::COLOR_PROFILE_AUTO == _options.outputColorProfile ||
                Graphics::Cineon::COLOR_PROFILE_FILM_PRINT == _options.outputColorProfile);
            _outputColorProfileLayout->labelForField(_outputBlackPointWidget)->setVisible(
                Graphics::Cineon::COLOR_PROFILE_AUTO == _options.outputColorProfile ||
                Graphics::Cineon::COLOR_PROFILE_FILM_PRINT == _options.outputColorProfile);
            _outputWhitePointWidget->setVisible(
                Graphics::Cineon::COLOR_PROFILE_AUTO == _options.outputColorProfile ||
                Graphics::Cineon::COLOR_PROFILE_FILM_PRINT == _options.outputColorProfile);
            _outputColorProfileLayout->labelForField(_outputWhitePointWidget)->setVisible(
                Graphics::Cineon::COLOR_PROFILE_AUTO == _options.outputColorProfile ||
                Graphics::Cineon::COLOR_PROFILE_FILM_PRINT == _options.outputColorProfile);
            _outputGammaWidget->setVisible(
                Graphics::Cineon::COLOR_PROFILE_AUTO == _options.outputColorProfile ||
                Graphics::Cineon::COLOR_PROFILE_FILM_PRINT == _options.outputColorProfile);
            _outputColorProfileLayout->labelForField(_outputGammaWidget)->setVisible(
                Graphics::Cineon::COLOR_PROFILE_AUTO == _options.outputColorProfile ||
                Graphics::Cineon::COLOR_PROFILE_FILM_PRINT == _options.outputColorProfile);
            _inputColorProfileWidget->setCurrentIndex(_options.inputColorProfile);
            _inputBlackPointWidget->setValue(_options.inputFilmPrint.black);
            _inputWhitePointWidget->setValue(_options.inputFilmPrint.white);
            _inputGammaWidget->setValue(_options.inputFilmPrint.gamma);
            _inputSoftClipWidget->setValue(_options.inputFilmPrint.softClip);
            _outputColorProfileWidget->setCurrentIndex(_options.outputColorProfile);
            _outputBlackPointWidget->setValue(_options.outputFilmPrint.black);
            _outputWhitePointWidget->setValue(_options.outputFilmPrint.white);
            _outputGammaWidget->setValue(_options.outputFilmPrint.gamma);
        }

        CineonWidgetPlugin::CineonWidgetPlugin(Core::CoreContext * context) :
            ImageIOWidgetPlugin(context)
        {}

        ImageIOWidget * CineonWidgetPlugin::createWidget(Graphics::ImageIO * plugin) const
        {
            return new CineonWidget(plugin, uiContext());
        }

        QString CineonWidgetPlugin::pluginName() const
        {
            return Graphics::Cineon::staticName;
        }

    } // namespace UI
} // namespace djv
