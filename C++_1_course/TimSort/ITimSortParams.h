#pragma once

typedef unsigned int ui32;



namespace timsort {
    enum EWhatMerge {
        WM_NoMerge, WM_MergeXY, WM_MergeYZ
    };

    struct ITimSortParams {
        // ���������� minRun �� n.
        virtual ui32 minRun(ui32 n) const = 0;
        // ����� �� ��������� �������, ���� � ����� ����� ��� ���� �������� lenX � lenY.
        virtual bool needMerge(ui32 lenX, ui32 lenY) const = 0;
        // ��� �������, ���� � ����� ��� ��������� ���� ����� ������� lenX, lenY � lenZ.
        virtual EWhatMerge whatMerge(ui32 lenX, ui32 lenY, ui32 lenZ) const = 0;
        // ����� � merge ������ ����������, ������� ��������� ������ ���������.           
        virtual ui32 getGallop() const = 0;
    };

    struct ITimSortParamsDefault : public ITimSortParams {
        ui32 minRun(ui32 n) const override {
            return 50;
        }
        bool needMerge(ui32 lenX, ui32 lenY) const override {
            return false;
        }
        EWhatMerge whatMerge(ui32 lenX, ui32 lenY, ui32 lenZ) const override {
            if (lenX >= lenY) {
                return WM_MergeXY;
            } else if (lenZ <= lenX + lenY) {
                return WM_MergeYZ;
            }
            return WM_NoMerge;
        }
        ui32 getGallop() const override {
            return 100000;
        }
    };
}

using timsort::ITimSortParams;
using timsort::ITimSortParamsDefault;
using timsort::EWhatMerge;